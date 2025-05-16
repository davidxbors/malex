package com.example.otpstealer

import android.os.Build
import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.annotation.RequiresApi
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.app.artful.ArtfulLibraryHelper
import com.example.otpstealer.ui.theme.OtpStealerTheme


class MainActivity : ComponentActivity() {

    companion object {
        init {
            System.loadLibrary("native-lib")
//            System.loadLibrary("artful")
//            ArtfulLibraryHelper.registerNativeMethodsForClass(MainActivity::class.java)
        }
    }

    external fun stringFromJNI(): String
    external fun getTime(): String
    // TODO: get rid of this
    external fun replaceLogEByObject(targetObject: Any?)

    fun test() {
        Log.e("ARTFUL", "I am benign!")
    }

    fun mal() {
        Log.d("ARTFUL", "I am malicious!")
    }

    fun run(): String {
        Log.d("ARTFUL", "lol")
        return "test"
    }

    @RequiresApi(Build.VERSION_CODES.O)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        Thread {
            val result = stringFromJNI()
            val time = getTime()
            test()
            test()
            test()
            Log.e("this_will", "not_work_as_expected_look_for_XORED_DEX")
            Log.d("NativeCode", time)
            Log.d("NativeCode", result)
        }.start()

        setContent {
            OtpStealerTheme {
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    Text(
                        text = "Hi! This is a harmless application!",
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