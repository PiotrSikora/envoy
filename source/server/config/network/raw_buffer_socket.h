#pragma once

#include "envoy/server/transport_socket_config.h"

#include "common/config/well_known_names.h"

namespace Envoy {
namespace Server {
namespace Configuration {

/**
 * Config registration for the raw buffer transport socket factory.
 * @see TransportSocketConfigFactory.
 */
class RawBufferSocketFactory : public virtual TransportSocketConfigFactory {
public:
  virtual ~RawBufferSocketFactory() {}
  std::string name() const override { return Config::TransportSocketNames::get().RAW_BUFFER; }
  ProtobufTypes::MessagePtr createEmptyConfigProto() override;
};

class UpstreamRawBufferSocketFactory : public UpstreamTransportSocketConfigFactory,
                                       public RawBufferSocketFactory {
public:
  Network::TransportSocketFactoryPtr
  createTransportSocketFactory(const Protobuf::Message& config,
                               TransportSocketFactoryContext& context) override;
};

class DownstreamRawBufferSocketFactory : public DownstreamTransportSocketConfigFactory,
                                         public RawBufferSocketFactory {
public:
  Network::TransportSocketFactoryPtr
  createTransportSocketFactory(const Protobuf::Message& config,
                               TransportSocketFactoryContext& context,
                               const std::vector<std::string>& server_names) override;
};

} // namespace Configuration
} // namespace Server
} // namespace Envoy
