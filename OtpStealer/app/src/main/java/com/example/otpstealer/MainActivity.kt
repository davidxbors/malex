package com.example.otpstealer

import android.os.Build
import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.annotation.RequiresApi
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.*
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import com.example.otpstealer.ui.theme.OtpStealerTheme
import androidx.compose.ui.unit.dp
import dalvik.system.InMemoryDexClassLoader
import java.nio.ByteBuffer


class MainActivity : ComponentActivity() {

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }

    external fun stringFromJNI(): String

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        val result = stringFromJNI()

//        val inputStream = resources.openRawResource(R.raw.classes)
//        val dexBytes = inputStream.readBytes()
//
//        val dexBuffer = ByteBuffer.allocateDirect(dexBytes.size)
//        dexBuffer.put(dexBytes)
//        dexBuffer.flip()
//
//        val classLoader = InMemoryDexClassLoader(dexBuffer, classLoader)
//        val pluginClass = classLoader.loadClass("com.example.MyPlugin")
//        val runMethod = pluginClass.getDeclaredMethod("run")
//        val pluginInstance = pluginClass.getDeclaredConstructor().newInstance()
//        val result = runMethod.invoke(pluginInstance) as String

        setContent {
            OtpStealerTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    Text(
                        text = result,
                        modifier = Modifier.padding(16.dp),
                        style = MaterialTheme.typography.bodyLarge
                    )
                }
            }
        }
    }
}

@Composable
fun Greeting(name: String, modifier: Modifier = Modifier) {
    Text(
        text = "Hello $name!",
        modifier = modifier
    )
}

@Preview(showBackground = true)
@Composable
fun GreetingPreview() {
    OtpStealerTheme {
        Greeting("Android")
    }
}