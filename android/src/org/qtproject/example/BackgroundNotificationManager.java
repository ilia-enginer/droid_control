package org.qtproject.example;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.util.Log;

/**
 * Управление фоновыми уведомлениями через AlarmManager.
 * AlarmManager работает независимо от Qt event loop и гарантирует
 * срабатывание даже когда приложение свёрнуто или в Doze mode.
 */
public class BackgroundNotificationManager {
    private static final String TAG = "BackgroundNotificationManager";
    private static final int ALARM_REQUEST_CODE = 1001;

    /**
     * Запустить повторяющиеся алармы для отправки уведомлений.
     * Вызывается из C++ через JNI.
     */
    public static void startRepeatingAlarm(Context context, int intervalMs, String[] phrases) {
        Log.d(TAG, "=== startRepeatingAlarm called ===");
        Log.d(TAG, "Interval: " + intervalMs + "ms (" + (intervalMs/1000) + " seconds)");
        Log.d(TAG, "Phrases count: " + (phrases != null ? phrases.length : 0));
        Log.d(TAG, "Android SDK: " + Build.VERSION.SDK_INT);
        
        if (phrases != null) {
            for (int i = 0; i < phrases.length; i++) {
                Log.d(TAG, "Phrase[" + i + "]: " + phrases[i]);
            }
        }
        
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        if (alarmManager == null) {
            Log.e(TAG, "ERROR: AlarmManager is null!");
            return;
        }

        // Сохраняем фразы в SharedPreferences
        NotificationAlarmReceiver.setPhrases(context, phrases);
        
        // Сбрасываем счётчик уведомлений
        NotificationAlarmReceiver.resetCounter();

        // Планируем первый аларм
        scheduleNextAlarm(context, intervalMs);
        
        Log.d(TAG, "First alarm scheduled successfully");
    }

    /**
     * Запланировать следующий аларм.
     * Для Android 6.0+ вызывается из NotificationAlarmReceiver после каждого срабатывания.
     */
    public static void scheduleNextAlarm(Context context, int intervalMs) {
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        if (alarmManager == null) {
            Log.e(TAG, "ERROR: AlarmManager is null in scheduleNextAlarm!");
            return;
        }

        Intent intent = new Intent(context, NotificationAlarmReceiver.class);
        intent.putExtra("interval", intervalMs);
        
        int flags = PendingIntent.FLAG_UPDATE_CURRENT;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            flags |= PendingIntent.FLAG_IMMUTABLE;
        }
        PendingIntent pendingIntent = PendingIntent.getBroadcast(
            context, ALARM_REQUEST_CODE, intent, flags);

        long triggerAtMillis = System.currentTimeMillis() + intervalMs;
        
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            // Android 6.0+: setExactAndAllowWhileIdle работает даже в Doze mode
            // Но не поддерживает repeating, поэтому перезапускаем вручную из receiver
            alarmManager.setExactAndAllowWhileIdle(
                AlarmManager.RTC_WAKEUP, triggerAtMillis, pendingIntent);
            Log.d(TAG, "Alarm scheduled with setExactAndAllowWhileIdle");
        } else if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            // Android 4.4-5.1: setExact для точного времени
            alarmManager.setExact(
                AlarmManager.RTC_WAKEUP, triggerAtMillis, pendingIntent);
            Log.d(TAG, "Alarm scheduled with setExact");
        } else {
            // Android < 4.4: setRepeating
            alarmManager.setRepeating(
                AlarmManager.RTC_WAKEUP, triggerAtMillis, intervalMs, pendingIntent);
            Log.d(TAG, "Alarm scheduled with setRepeating");
        }
        
        Log.d(TAG, "Next alarm at: " + new java.util.Date(triggerAtMillis));
        Log.d(TAG, "Current counter: " + NotificationAlarmReceiver.getCounter(context));
    }

    /**
     * Остановить повторяющиеся алармы.
     * Вызывается из C++ через JNI.
     */
    public static void stopRepeatingAlarm(Context context) {
        Log.d(TAG, "=== stopRepeatingAlarm called ===");
        
        AlarmManager alarmManager = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        if (alarmManager == null) {
            Log.e(TAG, "ERROR: AlarmManager is null!");
            return;
        }

        Intent intent = new Intent(context, NotificationAlarmReceiver.class);
        int flags = PendingIntent.FLAG_UPDATE_CURRENT;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            flags |= PendingIntent.FLAG_IMMUTABLE;
        }
        PendingIntent pendingIntent = PendingIntent.getBroadcast(
            context, ALARM_REQUEST_CODE, intent, flags);

        alarmManager.cancel(pendingIntent);
        pendingIntent.cancel();
        
        Log.d(TAG, "All alarms cancelled");
        Log.d(TAG, "Final counter: " + NotificationAlarmReceiver.getCounter(context));
    }
}
