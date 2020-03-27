#pragma once
#include "Clip.hxx"

struct FilterArguments final {
	self(InputMap, static_cast<const VSMap*>(nullptr));
	auto FetchValue(auto Broker, auto&& Name, auto Index) {
		auto ErrorState = 0;
		auto Value = Broker(InputMap, ExposeCString(Name), Index, &ErrorState);
		return std::tuple{ Value, ErrorState != 0 };
	}
	auto Fetch(auto& Parameter, auto&& Name) {
		using ParameterType = std::decay_t<decltype(Parameter)>;
		if constexpr (isinstance(Parameter, Clip)) {
			if (auto [Value, Error] = FetchValue(VaporGlobals::API->propGetNode, Name, 0); Error == false)
				Parameter = Clip{ Value };
		}
		else if constexpr (isinstance(Parameter, double) || isinstance(Parameter, float)) {
			if (auto [Value, Error] = FetchValue(VaporGlobals::API->propGetFloat, Name, 0); Error == false)
				Parameter = static_cast<ParameterType>(Value);
		}
		else if constexpr (isinstance(Parameter, std::int64_t) || isinstance(Parameter, int)) {
			if (auto [Value, Error] = FetchValue(VaporGlobals::API->propGetInt, Name, 0); Error == false)
				Parameter = static_cast<ParameterType>(Value);
		}
		else if constexpr (isinstance(Parameter, bool)) {
			if (auto [Value, Error] = FetchValue(VaporGlobals::API->propGetInt, Name, 0); Error == false)
				Parameter = !!Value;
		}
		else if constexpr (isinstance(Parameter, std::string)) {
			if (auto [Value, Error] = FetchValue(VaporGlobals::API->propGetData, Name, 0); Error == false)
				Parameter = std::string{ Value };
		}
	}
};

struct FilterOutputs final {
	self(OutputMap, static_cast<VSMap*>(nullptr));
	auto RaiseError(auto&& ErrorMessage) {
		VaporGlobals::API->setError(OutputMap, ExposeCString(ErrorMessage));
	}
};