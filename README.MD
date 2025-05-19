# MALEX

Collection of simple malware I write to learn more about the matter.

## OTPSTEALER

As the name suggests the goal of this malware is to still OTP codes from an infected user.

The malware has a `comicsans.ttf` file, which is in fact an encrypted DEX file.
Ar runtime, the `native-lib` library is loaded into memory, and `getTime()` is called.
`getTime()` decrypts `comicsans.ttf`, by retrieving the key from "a C2".
The key is `dummykeybutatleastitislong`.
The "C2" url must be replaced if the old one is not up.

The `comicsans.ttf` comes with a `run` function with the following source code:

```java
package com.example;

import android.util.Log;

public class MyClass {
    public static void run() {
        Log.d("XORED_DEX", "Hello from plugin!");
    }
}
```

`getTime()` also loads the `artful`[1] library.
This library allows us to replace swap functions at runtime.
This way, we swap `Log.e()` from the `android.util.Log` package with our `run` function.
This allows us to now execute our own custom code each time `Log.e()` is called.

In this case, the impact is not that big, we only change the log output, however, the plan is to run malicious code inside `run`.
This allows us to run malicious code, under the cover of seemingly benign functions, such as `Log.e()`

[1] https://github.com/LaurieWired/ARTful

The `run()` function can be found in `java-dev/MyClass.java`.
It is a simple function that checks if a notification might leak an OTP code.
If it does, it will encrypt it and send it to the C2.

### TODO:
- [x] implement actual otp stealing
- [ ] better encryption
- [ ] obfuscate
