package org.qtproject.example;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;

public class FeedbackActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_feedback);

        EditText input = findViewById(R.id.feedbackInput);
        Button send = findViewById(R.id.sendButton);

        // Получаем информацию об устройстве, переданную из C++
        String deviceInfo = getIntent().getStringExtra("device_info");
        if (deviceInfo == null) deviceInfo = "";

        final String finalDeviceInfo = deviceInfo;

        send.setOnClickListener(v -> {
            String userText = input.getText().toString().trim();
            if (userText.isEmpty()) return;

            // Склеиваем отзыв + информацию об устройстве
            String fullBody = userText
                + "\n\n--- Информация об устройстве ---\n"
                + finalDeviceInfo;

            Intent intent = new Intent(Intent.ACTION_SENDTO);
            intent.setData(Uri.parse("mailto:"));
            intent.putExtra(Intent.EXTRA_EMAIL, new String[]{"Ilia.zhuc@yandex.ru"}); // 👈 ваша почта
            intent.putExtra(Intent.EXTRA_SUBJECT, "Отзыв из приложения");
            intent.putExtra(Intent.EXTRA_TEXT, fullBody);

            startActivity(Intent.createChooser(intent, "Отправить отзыв..."));
            finish();
        });
    }
}
