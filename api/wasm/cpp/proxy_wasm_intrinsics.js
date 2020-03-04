mergeInto(LibraryManager.library, {
    proxy_get_configuration: function () {},
    proxy_log: function () {},
    proxy_set_tick_period_milliseconds: function () {},
    proxy_get_current_time_nanoseconds: function() {},
    proxy_get_status: function () {},
    proxy_get_property: function () {},
    proxy_set_property: function () {},
    proxy_continue_request: function () {},
    proxy_continue_response: function () {},
    proxy_clear_route_cache: function () {},
    proxy_add_header_map_value: function () {},
    proxy_get_header_map_value: function () {},
    proxy_get_header_map_pairs: function () {},
    proxy_get_header_map_size: function () {},
    proxy_get_shared_data: function () {},
    proxy_set_shared_data: function () {},
    proxy_register_shared_queue: function () {},
    proxy_resolve_shared_queue: function () {},
    proxy_enqueue_shared_queue: function () {},
    proxy_dequeue_shared_queue: function () {},
    proxy_replace_header_map_value: function () {},
    proxy_remove_header_map_value: function () {},
    proxy_get_buffer_bytes: function () {},
    proxy_get_buffer_status: function () {},
    proxy_set_buffer_bytes: function () {},
    proxy_http_call: function () {},
    proxy_define_metric: function () {},
    proxy_increment_metric: function () {},
    proxy_record_metric: function () {},
    proxy_get_metric: function () {},
    proxy_grpc_call : function () {},
    proxy_grpc_stream : function () {},
    proxy_grpc_send : function () {},
    proxy_grpc_close : function () {},
    proxy_grpc_cancel : function () {},
    proxy_send_local_response : function () {},
    proxy_set_effective_context : function () {},
    proxy_done: function () {},
    proxy_call_foreign_function: function () {},
});
