# Kaleidoscope Plugin API Internals

In this document we explain how the plugin system works behind the scenes. 

This is useful because there are some unorthodox solutions in play that make the code incredibly hard to untangle. It's an unavoidable side effect of employing a system that uses non-virtual functions, which lets us save large amounts of RAM.

Let's start at the top:

## `KALEIDOSCOPE_INIT_PLUGINS`

```c++
#define KALEIDOSCOPE_INIT_PLUGINS(...) _KALEIDOSCOPE_INIT_PLUGINS(__VA_ARGS__)
```

So far so good, this is pretty simple. The reason we use an indirection here is
because this is in `Kaleidoscope.h`, and we do not want the complexity of the
`_KALEIDOSCOPE_INIT_PLUGINS` macro here, nor do we want to move the macro to
another header, because it belongs to `Kaleidoscope.h`.

## `_KALEIDOSCOPE_INIT_PLUGINS`

```c++
#define _KALEIDOSCOPE_INIT_PLUGINS(...)                                       \
  namespace kaleidoscope_internal {                                           \
  struct EventDispatcher {                                                    \
                                                                              \
    template<typename EventHandler__, typename... Args__ >                    \
    static kaleidoscope::EventHandlerResult apply(Args__&&... hook_args) {    \
                                                                              \
      kaleidoscope::EventHandlerResult result;                                \
      MAP(_INLINE_EVENT_HANDLER_FOR_PLUGIN, __VA_ARGS__)                      \
                                                                              \
      return result;                                                          \
    }                                                                         \
  };                                                                          \
                                                                              \
  }                                                                           \
  _FOR_EACH_EVENT_HANDLER(_REGISTER_EVENT_HANDLER)
```

This is where things get interesting. This macro does two things:

 - It creates `kaleidoscope_internal::EventDispatcher`, a class with a single
   method, `apply`. This is a templated method. The template argument is the
   method `apply` calls. Therefore, `EventDispatcher::template apply<foo>` 
   resolves to  a function that calls the `foo` method of each plugin we list
   for `KALEIDOSCOPE_INIT_PLUGINS`. We'll see in a bit how this happens.

 - The other part creates overrides for the `Kaleidoscope::Hooks::` family of
   functions. These are wrappers around `EventDispatcher::template apply<foo>`.
   We have these so that higher level code does not need to be concerned with the
   implementation details. It can invoke the hooks as if they were
   ordinary functions.

## `_FOR_EACH_EVENT_HANDLER(_REGISTER_EVENT_HANDLER)`

Let's look at `_FOR_EACH_EVENT_HANDLER` and `_REGISTER_EVENT_HANDLER` first,
because that's easier to explain, and does not lead down another rabbit hole.

### `_REGISTER_EVENT_HANDLER`

```c++
#define _REGISTER_EVENT_HANDLER(                                          \
    HOOK_NAME, SHOULD_ABORT_ON_CONSUMED_EVENT, SIGNATURE, ARGS_LIST)      \
                                                                          \
  namespace kaleidoscope_internal {                                       \
                                                                          \
   struct EventHandler_##HOOK_NAME {                                      \
                                                                          \
      static bool shouldAbortOnConsumedEvent() {                          \
        return SHOULD_ABORT_ON_CONSUMED_EVENT;                            \
      }                                                                   \
                                                                          \
      template<typename Plugin__, typename... Args__>                     \
      static kaleidoscope::EventHandlerResult                             \
        call(Plugin__ &plugin, Args__&&... hook_args) {                   \
         _VALIDATE_EVENT_HANDLER_SIGNATURE(HOOK_NAME, Plugin__)           \
         return plugin.HOOK_NAME(hook_args...);                           \
      }                                                                   \
    };                                                                    \
                                                                          \
   } 	                                                                  \
                                                                          \
   namespace kaleidoscope {                                               \
                                                                          \
     EventHandlerResult Hooks::HOOK_NAME SIGNATURE {                      \
        return kaleidoscope_internal::EventDispatcher::template           \
        apply<kaleidoscope_internal::EventHandler_ ## HOOK_NAME>          \
             ARGS_LIST;                                                   \
      }                                                                   \
                                                                          \
   }
```

This looks big and scary, but in practice, it isn't all that bad. Nevertheless,
this is where the magic happens!

We create two things: `EventHandler_SomeThing` and `Hooks::SomeThing`, the
latter being a wrapper around the first, that uses `EventDispatcher::template
apply<>` discussed above.

Lets take `onSetup` as an example! For that, the above expands to:

```c++
namespace kaleidoscope_internal {

struct EventHandler_onSetup {
  static bool shouldAbortOnConsumedEvent() {
    return false;
  }

  template<typename Plugin__, typename... Args__>
  static kaleidoscope::EventHandlerResult
  call(Plugin__ &plugin, Args__&&... hook_args) {
    return plugin.onSetup(hook_args...);
  }
};

}

namespace kaleidoscope {

EventHandlerResult Hooks::onSetup() {
  return kaleidoscope_internal::EventDispatcher::template
         apply<kaleidoscope_internal::EventHandler_onSetup>();
}

}
```

This still looks scary... but please read a bit further, and it will all make
sense!

### `_FOR_EACH_EVENT_HANDLER`

This just evaluates its argument for each event handler supported by
Kaleidoscope core. Very simple macro expansion, which we will not expand here,
because that would take up a lot of space, and they all look the same (see
above).

## `EventDispatcher`

```c++
namespace kaleidoscope_internal {
struct EventDispatcher {
  template<typename EventHandler__, typename... Args__ >
  static kaleidoscope::EventHandlerResult apply(Args__&&... hook_args) {

    kaleidoscope::EventHandlerResult result;
    MAP(_INLINE_EVENT_HANDLER_FOR_PLUGIN, __VA_ARGS__)

    return result;
  }
};
```

This is where the other part of the magic happens, and we need to understand
this to be able to make sense of `_REGISTER_EVENT_HANDLER` above.

### `_INLINE_EVENT_HANDLER_FOR_PLUGIN`

In isolation, this is not very interesting, and is closely tied to
`EventDispatcher`. The definition is here so we can look at it while we learn
the details of `EventDispatcher` below.

```c++
#define _INLINE_EVENT_HANDLER_FOR_PLUGIN(PLUGIN)                     \
                                                                     \
   result = EventHandler__::call(PLUGIN, hook_args...);              \
                                                                     \
   if (EventHandler__::shouldAbortOnConsumedEvent() &&               \
       result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) { \
      return result;                                                 \
   }
```

### Back to `EventDispatcher`...

The `EventDispatcher` structure has a single method: `apply<>`, which requires an
event handler as its template argument. The macros calls the event
handler given in the template argument for each and every initialised plugin.
It's best explained with an example! Let's use two plugins, `SomePlugin` and
`ExampleEffect`:

```c++
namespace kaleidoscope_internal {
struct EventDispatcher {
  template<typename EventHandler__, typename... Args__ >
  static kaleidoscope::EventHandlerResult apply(Args__&&... hook_args) {

    kaleidoscope::EventHandlerResult result;

    result = EventHandler__::call(SomePlugin);
    if (EventHandler__::shouldAbortOnConsumedEvent() &&
        result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
        return result;
    }

    result = EventHandler__::call(ExampleEffect);
    if (EventHandler__::shouldAbortOnConsumedEvent() &&
        result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
        return result;
    }

    return result;
  }
};
```

See? It's unrolled! But how do we get from here to - say - calling the
`onSetup()` method of the plugins? Why, by way of `EventDispatcher::template
apply<EventHandler_onSetup>`! Lets see what happens when we do a call like that:

```c++
namespace kaleidoscope_internal {
struct EventDispatcher {
  template<typename EventHandler_onSetup, typename... Args__ >
  static kaleidoscope::EventHandlerResult apply(Args__&&... hook_args) {

    kaleidoscope::EventHandlerResult result;

    result = EventHandler_onSetup::call(SomePlugin);
    if (EventHandler_onSetup::shouldAbortOnConsumedEvent() &&
        result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
        return result;
    }

    result = EventHandler_onSetup::call(ExampleEffect);
    if (EventHandler_onSetup::shouldAbortOnConsumedEvent() &&
        result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
        return result;
    }

    return result;
  }
};
```

Because we call `EventHandler_onSetup::call` with the plugin as the first
argument, and because `call` is also a templated function, where the first
argument is templated, we get a method that is polymorphic on its first
argument. This means that for each and every plugin, we have a matching
`EventHandler_onSetup::call` that is tied to that plugin. *This* is the magic
that lets us use non-virtual methods.

## Exploring what the compiler does

Because all hooks are called via `kaleidoscope::Hooks::NAME`, let's explore how
the compiler will optimize the code for `onSetup`, assuming we use two plugins,
`SomePlugin` and `ExampleEffect`.

Our entry point is this:

```c++
return kaleidoscope::Hooks::onSetup();
```

`_REGISTER_EVENT_HANDLER` created `Hooks::onSetup()` for us:

```c++
EventHandlerResult kaleidoscope::Hooks::onSetup() {
  return kaleidoscope_internal::EventDispatcher::template
         apply<kaleidoscope_internal::EventHandler_onSetup>();
}
```

If we inline the call to `EventDispatcher::template apply<>`, we end up with the
following:

```c++
EventHandlerResult kaleidoscope::Hooks::onSetup() {
  kaleidoscope::EventHandlerResult result;

  result = EventHandler_onSetup::call(SomePlugin);
  if (EventHandler_onSetup::shouldAbortOnConsumedEvent() &&
    result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
    return result;
  }

  result = EventHandler_onSetup::call(ExampleEffect);
  if (EventHandler_onSetup::shouldAbortOnConsumedEvent() &&
    result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
    return result;
  }

  return result;
}
```

This is starting to look human readable, isn't it? But we can go further,
because `EventHandler::onSetup::call` and
`EventHandler_onSetup::shouldAbortOnConsumedEvent` are evaluated at compile-time
too!

```c++
EventHandlerResult kaleidoscope::Hooks::onSetup() {
  kaleidoscope::EventHandlerResult result;

  result = SomePlugin.onSetup();
  if (false &&
    result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
    return result;
  }

  result = ExampleEffect.onSetup();
  if (false &&
    result == kaleidoscope::EventHandlerResult::EVENT_CONSUMED) {
    return result;
  }

  return result;
}
```

Which in turn, may be optimized further to something like the following:

```c++
EventHandlerResult kaleidoscope::Hooks::onSetup() {
  kaleidoscope::EventHandlerResult result;

  result = SomePlugin.onSetup();
  result = ExampleEffect.onSetup();

  return result;
}
```

And this, is the end of the magic. This is roughly how much the code gets
transformed *at compile time*, so that at run-time, none of this indirection is
present.

## Summary

As you can see, there is a lot going on behind the scenes, and a combination of
template meta programming and pre-processor macros is used to accomplish our
goal. Following the code path as outlined above allows us to see what the
compiler sees (more or less), and inlining all the things that are done at 
compile-time provides us with the final code, which is pretty simple by that point.
