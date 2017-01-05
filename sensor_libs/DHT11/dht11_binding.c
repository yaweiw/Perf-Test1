#include "mico.h"
#include "dht11_operation.h"
#include "dht11_binding.h"

#define dht11_binding_log(format, ...) custom_log("DHT11_binding", format, ##__VA_ARGS__)

/* DHT11.prototype.readsync */
duk_ret_t dht11_readsync(duk_context *ctx)
{
    /* Stack at entry is: [ ] */

    /* Get access to the 'this' binding. */
    duk_push_this(ctx); /* -> stack: [ this ] */

    /* Read this.pin */
    duk_get_prop_string(ctx, -1, "pin");
    mico_gpio_t pin = duk_to_number(ctx, -1);

    uint8_t temp, humidity;
    IOT_DHT11_Read_Data(pin, &temp, &humidity);

    duk_push_object(ctx);                        /* -> stack: [ this returnObj] */
    duk_push_number(ctx, temp);                  /* -> stack: [ this returnObj temp] */
    duk_put_prop_string(ctx, -2, "temperature"); /* -> stack: [ this returnObj] */
    duk_push_number(ctx, humidity);              /* -> stack: [ this returnObj humidity] */
    duk_put_prop_string(ctx, -2, "humidity");    /* -> stack: [ this returnObj] */

    /* return object:
    {
        temperature: xxx,
        humidity: xxx
    }
    */
    return 1;
}

/* DHT11 constructor */
duk_ret_t dht11_constructor(duk_context *ctx)
{
    /* Stack at entry is: [ pin ] */

    /* All Duktape/C functions can be called both as constructors
     * ("new func()") and functions ("func()").  Sometimes objects
     * allow both calls, sometimes not.  Here we reject a normal
     * non-constructor call.
     */
    if (!duk_is_constructor_call(ctx))
    {
        return DUK_RET_TYPE_ERROR;
    }

    /* Get access to the default instance. */
    duk_push_this(ctx); /* -> stack: [ pin this ] */

    /* Set this.pin to pin. */
    duk_dup(ctx, 0);                          /* -> stack: [ pin this pin ] */
    mico_gpio_t pin = duk_to_number(ctx, -1); /* -> stack: [ pin this pin ] */
    duk_put_prop_string(ctx, -2, "pin");      /* -> stack: [ pin this ] */

    // Init DHT11 which is wired at specified GPIO pin
    IOT_DHT11_Init(pin);

    /* Return undefined: default instance will be used. */
    return 0;
}

/* Initialize DHT11 into global object. */
void dht11_binding_init(duk_context *ctx)
{
    /* Push constructor function; all Duktape/C functions are
     * "constructable" and can be called as 'new Foo()'.
     */
    duk_push_c_function(ctx, dht11_constructor, 1 /*nargs*/);

    /* Push DHT11.prototype object. */
    duk_push_object(ctx); /* -> stack: [ DHT11 proto ] */

    /* Set DHT11.prototype.readsync. */
    duk_push_c_function(ctx, dht11_readsync, 0 /*nargs*/);
    duk_put_prop_string(ctx, -2, "readsync");

    /* Set DHT11.prototype = proto */
    duk_put_prop_string(ctx, -2, "prototype"); /* -> stack: [ DHT11 ] */

    /* Finally, register DHT11 to the global object */
    duk_put_global_string(ctx, "DHT11"); /* -> stack: [ ] */
}
