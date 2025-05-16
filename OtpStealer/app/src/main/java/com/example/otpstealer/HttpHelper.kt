package com.example.otpstealer

import java.net.HttpURLConnection
import java.net.URL

import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext


object HttpHelper {
    @JvmStatic
    fun fetchUrl(urlString: String): String {
        return try {
            val url = URL(urlString)
            val connection = url.openConnection() as HttpURLConnection
            connection.requestMethod = "GET"
            connection.inputStream.bufferedReader().use { it.readText() }
        } catch (e: Exception) {
            e.printStackTrace()
            "Error: ${e.message}"
        }
    }
}
