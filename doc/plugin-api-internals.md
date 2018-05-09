# Kaleidoscope Plugin API Internals

This document attempts to explain how the plugin system works internally, behind
the scenes. We feel this is necessary, because there are some unorthodox
solutions in play, which make the code incredibly hard to untangle. This is an
unavoidable side-effect of employing a system that lets us use non-virtual
functions, and save large amounts of RAM thereby. It would be a lot simpler
without this feature, but alas, saving hundreds of bytes of RAM is something we
felt is worth the complexity.

Lets start at the top:

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
//
#define _KALEIDOSCOPE_INIT_PLUGINS(...)                                        \
         namespace kaleidoscope_internal {                                     \
         _DEFINE_HOOK_POINT(HookPoint, __VA_ARGS__)                            \
         } /* namespace kaleidoscope */                                        \
         _HOOKS_STATIC_METHODS_IMPLEMENTATION
```

This is where things get interesting. This macro does two things:

 - It creates `kaleidoscope_internal::HookPoint`, a class with a single method,
   `apply`. This is a templated method, the template argument is the method
   `apply` will call. Thus, `HookPoint::template apply<foo>` will resolve to a
   function that calls the `foo` method of each plugin we listed for
   `KALEIDOSCOPE_INIT_PLUGINS`. We'll see in a bit how this happens.

   This is the place where magic happens, where we make it possible to invoke a
   method of a class derived from `kaleidoscope::Plugin`, without that method
   needing to be `virtual`.

 - The other part creates overrides for the `Kaleidoscope::Hooks::` family of
   functions. These are wrappers around `HookPoint::template apply<foo>`. We
   have these so higher level code would not need to care about the
   implementation details, so that it can invoke the hooks as if they were
   ordinary functions.

## `_HOOKS_STATIC_METHODS_IMPLEMENTATION`

Lets look at `_HOOKS_STATIC_METHODS_IMPLEMENTATION` first, because that's easier
to explain, and does not lead down another rabbit hole.

It defines functions like this:

```c++
void Hooks::onSetup() {
  kaleidoscope_internal::HookPoint::template
     apply<kaleidoscope_internal::EventHandler_onSetup>();
}
```

The functions it defines are the ones in `kaleidoscope::Hooks`, as defined in
`kaleidoscope/hooks.h`. These are the functions Kaleidoscope will actually call.
All of these have a default empty implementation,
`_HOOKS_STATIC_METHODS_IMPLEMENTATION` overrides these defaults.

There is one method here for every hook. When new hooks are added, a wrapper
shall be added here too.

## `_DEFINE_HOOKPOINT`

This is where the magic continues. The macro will create a class that has an
`apply` method, with two arities: one without arguments and no returned value;
and another with some return value and a number of arguments. The template does
not specify the type of neither the return value, nor the function arguments.
Both of these will be deduced from the template argument.

To understand how `apply` works, we will first need to dive into `EventHandler`.

### Intermission: `_EVENT_HANDLER`

The macro creates a `EventHandler_something` class, with a single method,
`invoke`, which will call the `something` function. We do this wrapping so we
can assert that function signatures match. Without this enforcement, we would be
able to use plugins that have methods with the same name, but different
signatures than what `kaleidoscope::Plugin` permits, and that would result in
all kinds of undesired behaviour. The class also has a `shouldAbort` member, a
struct with a single `eval` method. This will be used later, outside of
`EventHandler` itself.

The `call` method does nothing else but assert on the signature (at compile
time!), and call the appropriate function of the plugin, which was passed to it
as an argument. Do note that the first argument to `call` is the plugin, with a
templated type. This part is what makes it possible to avoid having to declare
plugin methods `virtual`. At compile time, whatever type we pass to `call` in
its first argument, will be used, so inheritance rules no longer apply, we are
explicitly specifying the concrete class. This is why we also need the signature
check, because we can't rely on the type alone.

### HookPoint: apply - the void case

The void case is the easier one: it maps over all the used plugins, and calls
the appropriate method on them. This is done via `EventHandler`: every method is
wrapped in a `EventHandler`, and `apply` calls its `call` method. In the void
case, the main benefit - apart from not having to declare the methods
`virtual` - is enforced type safety.

### HookPoint: apply - the non-void case

The non-void case is more complicated: it has a return type, and arguments too.
Arguments are easy, we just pass them on as-is. The return type is extracted
from the template, and a local variable is used to hold it before returning. We
need this local variable, because invoking plugins in this case may terminate
early: this is where we use the `shouldAbort` member of `EventHandler`:

```c++
#define _CALL_HOOK_FOR_PLUGIN(PLUGIN)                                   \
  result = EventHandler__::call(PLUGIN, hook_args...);                  \
                                                                        \
  if (shouldAbort::eval(result)) {                                      \
     return result;                                                     \
  }
```

This is another reason we wrap the method calls in a struct: so we can have a
predicate associated with them.

In the end, `apply` will be a series of `_CALL_HOOK_FOR_PLUGIN` macros (that is
what `MAP` basically does, through a series of pre-processor macros).

# An example

Lets assume we have a very simple plugin:

```c++
class ExamplePlugin_ : public kaleidoscope::Plugin {
  ExamplePlugin_() {};
  void onSetup() { Serial.println("ExamplePlugin::onSetup()"); }
  void beforeReportingState() { Serial.println("ExamplePlugin::beforeReportingState()"); }
  void afterEachCycle() { Serial.println("ExamplePlugin::afterEachCycle()"); }
  bool onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
    Serial.println("ExamplePlugin::onKeyswitchEvent");
    return EVENT_CONTINUE;
  }
};

static ExamplePlugin_ ExamplePlugin;
```

## Expanding

Lets follow what happens when we do `KALEIDOSCOPE_INIT_PLUGINS(ExamplePlugin, ExamplePlugin)`!

### Expanding `KALEIDOSCOPE_INIT_PLUGINS`

```c++
_KALEIDOSCOPE_INIT_PLUGINS(ExamplePlugin, ExamplePlugin)
```

### Expanding `_KALEIDOSCOPE_INIT_PLUGINS`

```c++
namespace kaleidoscope_internal {
  _DEFINE_HOOKPOINT(HookPoint, ExamplePlugin, ExamplePlugin)
} /* namespace kaleidoscope */
_HOOKS_STATIC_METHODS_IMPLEMENTATION
```

### Expanding `_HOOKS_STATIC_METHODS_IMPLEMENTATION`

```c++
namespace kaleidoscope_internal {
  _DEFINE_HOOKPOINT(HookPoint, ExamplePlugin, ExamplePlugin)
}

namespace kaleidoscope {
  void Hooks::onSetup() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_onSetup>();
  }

  void Hooks::beforeEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeEachCycle>();
  }

  bool Hooks::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
    return kaleidoscope_internal::HookPoint::template
              apply<kaleidoscope_internal::EventHandler_onKeyswitchEvent>
                 (mappedKey, row, col, keyState);
  }

  void Hooks::beforeReportingState() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeReportingState>();
  }

  void Hooks::afterEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_afterEachCycle>();
  }
}
```

Nothing really surprising so far...

### Expanding `_DEFINE_HOOKPOINT`

```c++
namespace kaleidoscope_internal {
  struct HookPoint
  {
     template<typename EventHandler__,
              typename... Args__>
     static auto apply(Args__&&... hook_args)
                             -> typename EventHandler__::ReturnType {

        typedef typename EventHandler__::shouldAbort shouldAbort;
        typename EventHandler__::ReturnType result;

        MAP(_CALL_HOOK_FOR_PLUGIN, ExamplePlugin, ExamplePlugin)

        return result;
     }

     template<typename EventHandler__,
              typename... Args__>
     static void apply() {
        MAP(_CALL_EMPTY_ARGS_HOOK_FOR_PLUGIN, ExamplePlugin, ExamplePlugin)
     }
  };
}

namespace kaleidoscope {
  void Hooks::onSetup() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_onSetup>();
  }

  void Hooks::beforeEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeEachCycle>();
  }

  bool Hooks::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
    return kaleidoscope_internal::HookPoint::template
              apply<kaleidoscope_internal::EventHandler_onKeyswitchEvent>
                 (mappedKey, row, col, keyState);
  }

  void Hooks::beforeReportingState() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeReportingState>();
  }

  void Hooks::afterEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_afterEachCycle>();
  }
}
```

### Expanding `MAP`

```c++
namespace kaleidoscope_internal {
  struct HookPoint
  {
     template<typename EventHandler__,
              typename... Args__>
     static auto apply(Args__&&... hook_args)
                             -> typename EventHandler__::ReturnType {

        typedef typename EventHandler__::shouldAbort shouldAbort;
        typename EventHandler__::ReturnType result;

        _CALL_HOOK_FOR_PLUGIN(ExamplePlugin)
        _CALL_HOOK_FOR_PLUGIN(ExamplePlugin)

        return result;
     }

     template<typename EventHandler__,
              typename... Args__>
     static void apply() {
        _CALL_EMPTY_ARGS_HOOK_FOR_PLUGIN(ExamplePlugin)
        _CALL_EMPTY_ARGS_HOOK_FOR_PLUGIN(ExamplePlugin)
     }
  };
}

namespace kaleidoscope {
  void Hooks::onSetup() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_onSetup>();
  }

  void Hooks::beforeEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeEachCycle>();
  }

  bool Hooks::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
    return kaleidoscope_internal::HookPoint::template
              apply<kaleidoscope_internal::EventHandler_onKeyswitchEvent>
                 (mappedKey, row, col, keyState);
  }

  void Hooks::beforeReportingState() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeReportingState>();
  }

  void Hooks::afterEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_afterEachCycle>();
  }
}
```

### Expanding `CALL_*`

```c++
namespace kaleidoscope_internal {
  struct HookPoint
  {
     template<typename EventHandler__,
              typename... Args__>
     static auto apply(Args__&&... hook_args)
                             -> typename EventHandler__::ReturnType {

        typedef typename EventHandler__::shouldAbort shouldAbort;
        typename EventHandler__::ReturnType result;

        result = EventHandler__::call(ExamplePlugin, hook_args...);
        if (shouldAbort::eval(hook_return_val)) {
          return result;
        }

        result = EventHandler__::call(ExamplePlugin, hook_args...);
        if (shouldAbort::eval(hook_return_val)) {
          return result;
        }

        return result;
     }

     template<typename EventHandler__,
              typename... Args__>
     static void apply() {
        EventHandler__::call(ExamplePlugin);
        EventHandler__::call(ExamplePlugin);
     }
  };
}

namespace kaleidoscope {
  void Hooks::onSetup() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_onSetup>();
  }

  void Hooks::beforeEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeEachCycle>();
  }

  bool Hooks::onKeyswitchEvent(Key &mappedKey, byte row, byte col, uint8_t keyState) {
    return kaleidoscope_internal::HookPoint::template
              apply<kaleidoscope_internal::EventHandler_onKeyswitchEvent>
                 (mappedKey, row, col, keyState);
  }

  void Hooks::beforeReportingState() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_beforeReportingState>();
  }

  void Hooks::afterEachCycle() {
    kaleidoscope_internal::HookPoint::template
       apply<kaleidoscope_internal::EventHandler_afterEachCycle>();
  }
}
```

## Calling

Now that we have the macros expanded, lets see what happens when we call a hook!
Remember that our entry point is in `Kaleidoscope::Hooks`. We'll first follow
`Kaleidoscope::Hooks::onSetup()`, then `Kaleidoscope::Hooks::onKeyswitchEvent`.

### `Kaleidoscope::Hooks::init()`

First, we call `kaleidoscope_internal::HookPoint::template
apply<EventHandler_onSetup>()`. Remember that `apply<EventHandler_onSetup>` is a
templated function, so - at compile time - this expands to:

```c++
static void apply<EventHandler_onSetup>() {
  EventHandler_onSetup::call(ExamplePlugin);
  EventHandler_onSetup::call(ExamplePlugin);
}
```

And `EventHandler_onSetup::call` expands to:

```c++
static void call(ExamplePlugin_ &plugin) {
  return plugin.onSetup();
}
```

Considering that `EventHandler_onSetup` can be inlined, our
`apply<EventHandler_onSetup>` function is effectively this now:

```c++
static void apply<EventHandler_onSetup>() {
  ExamplePlugin.onSetup();
  ExamplePlugin.onSetup();
}
```

Since this can also be inlined, our `Kaleidoscope::Hooks::onSetup` becomes:

```c++
void Hooks::onSetup() {
  ExamplePlugin.onSetup();
  ExamplePlugin.onSetup();
}
```

### `Kaleidoscope::Hooks::onKeyswitchEvent()`

The only difference here is that we expand `apply<EventHandler_onKeyswitchEvent>`
differently:

```c++
static auto apply(Args__&&... hook_args)
                        -> typename HookTask__::ReturnType {

   typedef typename EventHandler__::shouldAbort shouldAbort;
   typename EventHandler__::ReturnType result;

   result = EventHandler__::call(ExamplePlugin, hook_args...);
   if (shouldAbort::eval(result)) {
     return result;
   }

   result = EventHandler__::call(ExamplePlugin, hook_args...);
   if (shouldAbort::eval(result)) {
     return result;
   }

   return result;
}
```

Which in turn becomes:

```c++
static bool apply(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  bool result;

  result = EventHandler_onKeyswitchEvent::call(ExamplePlugin, mappedKey, row, col, keyState);
  if (AbortIfFalse::eval(result)) {
    return result;
  }

  result = EventHandler_onKeyswitchEvent(ExamplePlugin, mappedKey, row, col, keyState);
  if (AbortIfFalse::eval(result)) {
    return result;
  }

  return result;
}
```

If we expand the predicate, and simplify the code a bit, as the compiler would,
we end up with:

```c++
static bool apply(Key &mappedKey, byte row, byte col, uint8_t keyState) {
  bool result;

  result = ExamplePlugin.onKeyswitchEvent(mappedKey, row, col, keyState);
  if (!result) {
    return result;
  }

  result = ExamplePlugin.onKeyswitchEvent(mappedKey, row, col, keyState);
  if (!result) {
    return result;
  }

  return result;
}
```

## Summary

As we can see, there is a lot going on behind the scenes, and a combination of
template meta programming and pre-processor macros is used to accomplish our
goal. But following the code path like we did above allows us to see what the
compiler sees (more or less), and inlining all the things that are done
compile-time gives us the final code, which is pretty simple by that point.
