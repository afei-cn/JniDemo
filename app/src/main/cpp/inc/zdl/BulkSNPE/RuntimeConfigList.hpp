//==============================================================================
//
//  Copyright (c) 2019 Qualcomm Technologies, Inc.
//  All Rights Reserved.
//  Confidential and Proprietary - Qualcomm Technologies, Inc.
//
//==============================================================================
#ifndef BULKSNPE_RUNTIMECONFIGLIST_HPP
#define BULKSNPE_RUNTIMECONFIGLIST_HPP

#include <iostream>
#include "DlSystem/DlEnums.hpp"
#include "DlContainer/IDlContainer.hpp"
#include "DlSystem/ZdlExportDefine.hpp"
#include "DlSystem/RuntimeList.hpp"

namespace zdl {
namespace BulkSNPE {

/** @addtogroup c_plus_plus_apis C++
@{ */

/**
  * @brief .
  *
  * The structure for configuring a BulkSNPE runtime
  *
  */
struct ZDL_EXPORT RuntimeConfig final {
   zdl::DlSystem::Runtime_t runtime;
   zdl::DlSystem::RuntimeList runtimeList;
   zdl::DlSystem::PerformanceProfile_t perfProfile;
   bool enableCPUFallback;
   RuntimeConfig() {}
   RuntimeConfig(const RuntimeConfig& other)
   {
       runtime           = other.runtime;
       runtimeList       = other.runtimeList;
       perfProfile       = other.perfProfile;
       enableCPUFallback = other.enableCPUFallback;
   }

   RuntimeConfig& operator=(const RuntimeConfig &other)
   {
       this->runtimeList       = other.runtimeList;
       this->runtime           = other.runtime;
       this->perfProfile       = other.perfProfile;
       this->enableCPUFallback = other.enableCPUFallback;
       return *this;
   }

   ~RuntimeConfig() {}

};

/**
* @brief .
*
* The class for creating a RuntimeConfig container.
*
*/
class ZDL_EXPORT RuntimeConfigList final
{
public:
   RuntimeConfigList();
   RuntimeConfigList(const size_t size);
   void push_back(const RuntimeConfig &runtimeConfig);
   RuntimeConfig& operator[](const size_t index);
   RuntimeConfigList& operator =(const RuntimeConfigList &other);
   size_t size() const noexcept;
   size_t capacity() const noexcept;
   void clear() noexcept;
   ~RuntimeConfigList() = default;

private:
   void swap(const RuntimeConfigList &other);
   std::vector<RuntimeConfig> m_runtimeConfigs;

};
/** @} */ /* end_addtogroup c_plus_plus_apis C++ */

} // namespace BulkSNPE
} // namespace zdl
#endif //BULKSNPE_RUNTIMECONFIGLIST_HPP
