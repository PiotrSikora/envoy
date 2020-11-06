#include "extensions/filters/network/wasm/wasm_filter.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace Wasm {

using Envoy::Extensions::Common::Wasm::WasmHandle;

FilterConfig::FilterConfig(const envoy::extensions::filters::network::wasm::v3::Wasm& config,
                           Server::Configuration::FactoryContext& context)
    : tls_slot_(ThreadLocal::TypedSlot<WasmHandle>::makeUnique(context.threadLocal())) {
  plugin_ = std::make_shared<Common::Wasm::Plugin>(
      config.config().name(), config.config().root_id(), config.config().vm_config().vm_id(),
      config.config().vm_config().runtime(),
      Common::Wasm::anyToBytes(config.config().configuration()), config.config().fail_open(),
      context.direction(), context.localInfo(), &context.listenerMetadata());

  auto plugin = plugin_;
  auto callback = [plugin, this](Common::Wasm::WasmHandleSharedPtr base_wasm) {
    // NB: the Slot set() call doesn't complete inline, so all arguments must outlive this call.
    tls_slot_->set([base_wasm, plugin](Event::Dispatcher& dispatcher) {
      return Common::Wasm::getOrCreateThreadLocalWasm(base_wasm, plugin, dispatcher);
    });
  };

  if (!Common::Wasm::createWasm(
          config.config().vm_config(), plugin_, context.scope().createScope(""),
          context.clusterManager(), context.initManager(), context.dispatcher(), context.api(),
          context.lifecycleNotifier(), remote_data_provider_, std::move(callback))) {
    throw Common::Wasm::WasmException(
        fmt::format("Unable to create Wasm network filter {}", plugin->name_));
  }
}

FilterConfig::~FilterConfig() {
  if (tls_slot_->currentThreadRegistered()) {
    // Start graceful shutdown of Wasm plugin, unless Envoy is already shutting down.
    tls_slot_->runOnAllThreads([plugin = plugin_](OptRef<WasmHandle> handle) {
      if (handle.has_value()) {
        handle->wasm()->startShutdown(plugin);
      }
    });
  }
}

} // namespace Wasm
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
