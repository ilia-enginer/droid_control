package org.qtproject.example;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.util.Log;
import org.qtproject.qt.android.bindings.QtService;


public class QtAndroidService extends QtService
{
    private static final String TAG = "QtAndroidService";
    private static final int NOTIFICATION_ID = 7001;
    private static final String CHANNEL_ID = "droid_control_transfer";
    public static QtAndroidService INSTANCE;

    @Override
    public void onCreate() {
        super.onCreate();
        INSTANCE = this;
        createChannel();
        Log.i(TAG, "Creating Service");
    }

    @Override
    public void onDestroy() {
        stopForeground(true);
        super.onDestroy();
        INSTANCE = null;
        Log.i(TAG, "Destroying Service");
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        super.onStartCommand(intent, flags, startId);
        startForeground(NOTIFICATION_ID, buildNotification());
        return android.app.Service.START_STICKY;
    }

    private void createChannel() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
            if (manager != null) {
                NotificationChannel channel = new NotificationChannel(
                        CHANNEL_ID,
                        "Droid Control",
                        NotificationManager.IMPORTANCE_LOW);
                manager.createNotificationChannel(channel);
            }
        }
    }

    private Notification buildNotification() {
        Context context = getApplicationContext();
        Intent intent = new Intent(context, org.qtproject.qt.android.bindings.QtActivity.class);
        intent.setAction(Intent.ACTION_MAIN);
        intent.addCategory(Intent.CATEGORY_LAUNCHER);
        intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
        int flags = Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ? PendingIntent.FLAG_IMMUTABLE : 0;
        PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, flags);
        Notification.Builder builder;
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            builder = new Notification.Builder(context, CHANNEL_ID);
        } else {
            builder = new Notification.Builder(context);
            builder.setPriority(Notification.PRIORITY_LOW);
        }
        builder.setContentTitle("Droid Control");
        builder.setContentText("Приложение работает в фоне");
        builder.setSmallIcon(android.R.drawable.stat_sys_upload);
        builder.setContentIntent(pendingIntent);
        builder.setOnlyAlertOnce(true);
        builder.setOngoing(true);       // уведомление нельзя смахнуть, оно управляется приложением
        builder.setShowWhen(false);     // без отображения временной метки
        return builder.build();
    }

    public static void startQtAndroidService(Context context)
    {
        if (context != null) {
            Intent serviceIntent = new Intent(context, QtAndroidService.class);
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
                context.startForegroundService(serviceIntent);
            } else {
                context.startService(serviceIntent);
            }
            Log.d(TAG, "Android Service Start");
        }
    }

    public static void stopQtAndroidService(Context context)
    {
        if (context != null) {
            Intent serviceIntent = new Intent(context, QtAndroidService.class);
            context.stopService(serviceIntent);
            Log.d(TAG, "Android Service Stop");
        }
    }
}
