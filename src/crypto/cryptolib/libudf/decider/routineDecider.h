

#pragma once
#include "statusReport.h"


#include <unordered_map>
class RotineDecider {

    using ExecutionPath = StatusReport::ExecutionPath;
    using UdfKind = StatusReport::UdfKind;

    struct UDFExecutionStatistics
    {
        struct ExecutionPathStatistics {
            ExecutionPath path;
            

        };

        UdfKind udfKind;

        ExecutionPathStatistics eps[ExecutionPath::COUNT];     // 
    };

    // std::unordered_map<int, UDFExecutionStatistics> _;
};