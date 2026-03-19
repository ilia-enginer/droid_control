package org.qtproject.example;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.PowerManager;
import android.provider.Settings;
import android.util.Log;

public class NotificationClient
{
    private static final String TAG           = "NotificationClient";
    private static final String CHANNEL_ID    = "droid_control_channel";
    private static final String CHANNEL_NAME  = "Droid Control";
    private static final int    NOTIFICATION_ID = 1001;

    /** Отправить уведомление. Вызывается из C++ через JNI. */
    public static void notify(Context context, String message) {
        Log.d(TAG, "=== notify() START ===");
        Log.d(TAG, "Thread: " + Thread.currentThread().getName() + " (ID: " + Thread.currentThread().getId() + ")");
        Log.d(TAG, "Message: " + message);
        Log.d(TAG, "Context: " + (context != null ? context.getClass().getName() : "NULL"));
        
        try {
            if (context == null) {
                Log.e(TAG, "ERROR: Context is NULL!");
                return;
            }
            
            Context appContext = context.getApplicationContext();
            Log.d(TAG, "AppContext obtained: " + (appContext != null ? appContext.getClass().getName() : "NULL"));
            
            if (appContext == null) {
                Log.e(TAG, "ERROR: ApplicationContext is NULL!");
                return;
            }

            NotificationManager notificationManager =
                    (NotificationManager) appContext.getSystemService(Context.NOTIFICATION_SERVICE);
            Log.d(TAG, "NotificationManager obtained: " + (notificationManager != null));

            if (notificationManager == null) {
                Log.e(TAG, "ERROR: NotificationManager is null");
                return;
            }

            // Канал IMPORTANCE_HIGH — уведомление показывается в виде плашки
            // ("heads-up"), даже когда экран заблокирован или приложение свёрнуто.
            // IMPORTANCE_DEFAULT показывает только иконку в панели без popup.
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                Log.d(TAG, "Android O+, checking notification channel...");
                NotificationChannel channel = notificationManager.getNotificationChannel(CHANNEL_ID);
                if (channel == null) {
                    Log.d(TAG, "Channel doesn't exist, creating new channel...");
                    channel = new NotificationChannel(
                            CHANNEL_ID,
                            CHANNEL_NAME,
                            NotificationManager.IMPORTANCE_HIGH  // было DEFAULT — менялось на HIGH
                    );
                    channel.setDescription("Уведомления от Droid Control");
                    channel.enableVibration(true);
                    notificationManager.createNotificationChannel(channel);
                    Log.d(TAG, "Notification channel created (IMPORTANCE_HIGH)");
                } else {
                    Log.d(TAG, "Channel already exists, importance: " + channel.getImportance());
                }
            }

            Log.d(TAG, "Loading icon resource...");
            Bitmap icon = BitmapFactory.decodeResource(appContext.getResources(), R.drawable.icon);
            Log.d(TAG, "Icon loaded: " + (icon != null));

            Log.d(TAG, "Building notification...");
            Notification.Builder builder;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                builder = new Notification.Builder(appContext, CHANNEL_ID);
            } else {
                builder = new Notification.Builder(appContext);
            }

            builder.setSmallIcon(R.drawable.icon)
                   .setLargeIcon(icon)
                   .setContentTitle("Droid Control")
                   .setContentText(message)
                   .setPriority(Notification.PRIORITY_HIGH)  // для API < 26
                   .setDefaults(Notification.DEFAULT_ALL)
                   .setAutoCancel(true);

            Notification notification = builder.build();
            Log.d(TAG, "Notification built successfully");
            
            Log.d(TAG, "Calling notificationManager.notify() with ID: " + NOTIFICATION_ID);
            notificationManager.notify(NOTIFICATION_ID, notification);
            Log.d(TAG, "=== Notification sent OK: " + message + " ===");

        } catch (Exception e) {
            Log.e(TAG, "=== EXCEPTION in notify() ===", e);
            e.printStackTrace();
        }
    }

    /**
     * Запросить у пользователя разрешение исключить приложение из батарейной
     * оптимизации. Без этого Android (особенно Doze mode) может блокировать
     * background-задачи даже у foreground services.
     *
     * Вызывается из C++ через JNI при старте тестового таймера.
     */
    public static void requestIgnoreBatteryOptimizations(Context context) {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) return;
        try {
            PowerManager pm = (PowerManager) context.getSystemService(Context.POWER_SERVICE);
            if (pm == null) return;

            String pkg = context.getPackageName();
            if (!pm.isIgnoringBatteryOptimizations(pkg)) {
                Log.d(TAG, "Requesting battery optimization ignore for: " + pkg);
                Intent intent = new Intent(Settings.ACTION_REQUEST_IGNORE_BATTERY_OPTIMIZATIONS);
                intent.setData(Uri.parse("package:" + pkg));
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                context.startActivity(intent);
            } else {
                Log.d(TAG, "Battery optimization already ignored for: " + pkg);
            }
        } catch (Exception e) {
            Log.e(TAG, "Failed to request battery optimization ignore", e);
        }
    }
}
