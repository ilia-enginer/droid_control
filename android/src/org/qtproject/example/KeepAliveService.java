package org.qtproject.example;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.util.Log;

/**
 * KeepAliveService — простой Foreground Service (НЕ Qt-сервис), работает
 * в главном процессе приложения. Вызов startForeground() переводит главный
 * процесс в режим "foreground service priority":
 *   - Android не убивает процесс
 *   - Qt event loop работает без throttling
 *   - QTimer и JNI-вызовы работают корректно из фона
 *   
 * Также содержит Handler для периодических вызовов C++ callback.
 */
public class KeepAliveService extends Service
{
    private static final String TAG           = "KeepAliveService";
    private static final String CHANNEL_ID    = "droid_keepalive_channel";
    private static final int    NOTIFICATION_ID = 2001;
    
    private Handler handler;
    private Runnable timerRunnable;
    private int intervalMs = 10000;
    private boolean timerRunning = false;

    @Override
    public void onCreate() {
        super.onCreate();
        Log.i(TAG, "=== KeepAliveService onCreate() ===");
        handler = new Handler(Looper.getMainLooper());
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(TAG, "=== KeepAliveService onStartCommand() ===");
        Log.i(TAG, "Thread: " + Thread.currentThread().getName());
        
        // Проверяем, нужно ли запустить таймер
        if (intent != null && intent.hasExtra("start_timer")) {
            boolean startTimer = intent.getBooleanExtra("start_timer", false);
            if (startTimer) {
                intervalMs = intent.getIntExtra("interval", 10000);
                Log.i(TAG, "Starting notification timer with interval: " + intervalMs + "ms");
                startNotificationTimer();
            } else {
                Log.i(TAG, "Stopping notification timer");
                stopNotificationTimer();
            }
        }
        
        Notification notification = buildForegroundNotification();
        Log.i(TAG, "Calling startForeground() with ID: " + NOTIFICATION_ID);
        
        try {
            startForeground(NOTIFICATION_ID, notification);
            Log.i(TAG, "KeepAliveService started in foreground successfully");
        } catch (Exception e) {
            Log.e(TAG, "ERROR: Failed to start foreground service", e);
        }
        
        // START_STICKY: если Android убьёт сервис, он перезапустится
        return Service.START_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        stopNotificationTimer();
        stopForeground(true);
        Log.i(TAG, "KeepAliveService destroyed");
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private Notification buildForegroundNotification() {
        NotificationManager nm =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O && nm != null) {
            NotificationChannel ch = nm.getNotificationChannel(CHANNEL_ID);
            if (ch == null) {
                ch = new NotificationChannel(
                        CHANNEL_ID,
                        "Droid Control фон",
                        NotificationManager.IMPORTANCE_LOW   // без звука, только иконка
                );
                ch.setDescription("Фоновая работа Droid Control");
                nm.createNotificationChannel(ch);
            }
        }

        Notification.Builder builder;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            builder = new Notification.Builder(this, CHANNEL_ID);
        } else {
            builder = new Notification.Builder(this);
        }

        return builder
                .setSmallIcon(R.drawable.icon)
                .setContentTitle("Droid Control")
                .setContentText("Приложение работает в фоне")
                .setPriority(Notification.PRIORITY_LOW)
                .build();
    }

    // ─── Таймер для периодических вызовов C++ callback ────────────────────────

    private void startNotificationTimer() {
        if (timerRunning) {
            Log.w(TAG, "Timer already running");
            return;
        }
        
        timerRunning = true;
        timerRunnable = new Runnable() {
            @Override
            public void run() {
                if (!timerRunning) {
                    return;
                }
                
                Log.d(TAG, "Timer tick, calling C++ callback...");
                
                // Вызываем C++ callback через JNI
                // C++ код сам решит какой текст отправить в уведомлении
                try {
                    nativeOnTimerTick();
                } catch (Exception e) {
                    Log.e(TAG, "ERROR calling C++ callback", e);
                }
                
                // Перезапускаем через intervalMs
                if (timerRunning) {
                    handler.postDelayed(this, intervalMs);
                }
            }
        };
        
        // Первый запуск
        handler.postDelayed(timerRunnable, intervalMs);
        Log.i(TAG, "Notification timer started");
    }
    
    private void stopNotificationTimer() {
        if (!timerRunning) {
            return;
        }
        
        timerRunning = false;
        if (handler != null && timerRunnable != null) {
            handler.removeCallbacks(timerRunnable);
        }
        Log.i(TAG, "Notification timer stopped");
    }
    
    /**
     * Native метод, который будет вызываться из Java.
     * Должен быть реализован в C++ коде.
     */
    private native void nativeOnTimerTick();

    // ─── Вспомогательные статические методы (вызываются из C++ через JNI) ────

    public static void startService(Context context) {
        Intent intent = new Intent(context, KeepAliveService.class);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            context.startForegroundService(intent);
        } else {
            context.startService(intent);
        }
        Log.d(TAG, "startService called");
    }
    
    public static void startServiceWithTimer(Context context, int intervalMs) {
        Intent intent = new Intent(context, KeepAliveService.class);
        intent.putExtra("start_timer", true);
        intent.putExtra("interval", intervalMs);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            context.startForegroundService(intent);
        } else {
            context.startService(intent);
        }
        Log.d(TAG, "startServiceWithTimer called, interval: " + intervalMs);
    }
    
    public static void stopServiceTimer(Context context) {
        Intent intent = new Intent(context, KeepAliveService.class);
        intent.putExtra("start_timer", false);
        context.startService(intent);
        Log.d(TAG, "stopServiceTimer called");
    }

    public static void stopService(Context context) {
        Intent intent = new Intent(context, KeepAliveService.class);
        context.stopService(intent);
        Log.d(TAG, "stopService called");
    }
}
