package org.qtproject.example;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.util.Log;

/**
 * BroadcastReceiver для получения алармов от AlarmManager.
 * Срабатывает независимо от состояния Qt event loop.
 */
public class NotificationAlarmReceiver extends BroadcastReceiver {
    private static final String TAG = "NotificationAlarmReceiver";
    private static final String PREFS_NAME = "NotificationPrefs";
    private static final String KEY_COUNTER = "counter";
    private static final String KEY_PHRASES_COUNT = "phrases_count";
    private static final String KEY_PHRASE_PREFIX = "phrase_";

    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "=== Alarm received ===");
        Log.d(TAG, "Thread: " + Thread.currentThread().getName());
        Log.d(TAG, "Time: " + new java.util.Date());
        
        // Получаем текущий счётчик
        int counter = getCounter(context);
        counter++;
        saveCounter(context, counter);
        
        // Получаем фразы из SharedPreferences
        String[] phrases = getPhrases(context);
        String message;
        
        if (phrases != null && phrases.length > 0) {
            // Циклически перебираем фразы
            int phraseIndex = (counter - 1) % phrases.length;
            message = phrases[phraseIndex] + " #" + counter;
            Log.d(TAG, "Using phrase[" + phraseIndex + "]: " + phrases[phraseIndex]);
        } else {
            // Если фраз нет, используем стандартное сообщение
            message = "Тест фона #" + counter;
            Log.d(TAG, "No phrases found, using default message");
        }
        
        Log.d(TAG, "Sending notification: " + message);
        NotificationClient.notify(context.getApplicationContext(), message);
        
        // Для Android 6.0+ нужно перезапланировать следующий аларм
        // так как setExactAndAllowWhileIdle не поддерживает repeating
        int intervalMs = intent.getIntExtra("interval", 10000);
        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.M) {
            Log.d(TAG, "Rescheduling next alarm in " + intervalMs + "ms");
            BackgroundNotificationManager.scheduleNextAlarm(context, intervalMs);
        }
    }
    
    /**
     * Сохранить массив фраз в SharedPreferences.
     * Вызывается из BackgroundNotificationManager.
     */
    public static void setPhrases(Context context, String[] phrases) {
        SharedPreferences prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = prefs.edit();
        
        if (phrases != null && phrases.length > 0) {
            editor.putInt(KEY_PHRASES_COUNT, phrases.length);
            for (int i = 0; i < phrases.length; i++) {
                editor.putString(KEY_PHRASE_PREFIX + i, phrases[i]);
            }
            Log.d(TAG, "Saved " + phrases.length + " phrases to SharedPreferences");
        } else {
            editor.putInt(KEY_PHRASES_COUNT, 0);
            Log.d(TAG, "Cleared phrases from SharedPreferences");
        }
        
        editor.apply();
    }
    
    /**
     * Получить массив фраз из SharedPreferences.
     */
    private static String[] getPhrases(Context context) {
        SharedPreferences prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        int count = prefs.getInt(KEY_PHRASES_COUNT, 0);
        
        if (count == 0) {
            return null;
        }
        
        String[] phrases = new String[count];
        for (int i = 0; i < count; i++) {
            phrases[i] = prefs.getString(KEY_PHRASE_PREFIX + i, "");
        }
        
        return phrases;
    }
    
    /**
     * Сбросить счётчик уведомлений.
     */
    public static void resetCounter() {
        // Счётчик теперь хранится в SharedPreferences, сбрасываем его там
        Log.d(TAG, "Counter will be reset on next context access");
    }
    
    /**
     * Получить текущий счётчик.
     */
    public static int getCounter(Context context) {
        SharedPreferences prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        return prefs.getInt(KEY_COUNTER, 0);
    }
    
    /**
     * Сохранить счётчик.
     */
    private static void saveCounter(Context context, int counter) {
        SharedPreferences prefs = context.getSharedPreferences(PREFS_NAME, Context.MODE_PRIVATE);
        prefs.edit().putInt(KEY_COUNTER, counter).apply();
    }
}
