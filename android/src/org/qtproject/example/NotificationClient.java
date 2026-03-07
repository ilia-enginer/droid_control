package org.qtproject.example;

import android.app.Notification;
import android.app.NotificationManager;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.BitmapFactory;
import android.app.NotificationChannel;
import android.content.Intent;
import android.os.Build;


import android.annotation.SuppressLint;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;



public class NotificationClient
{

//    public final static String TAG = "ScreenCaptureServices";

//    private String NOTIFICATION_CHANNEL_ID = "AudioCaptureService_nofity";
//    private String NOTIFICATION_CHANNEL_NAME = "AudioCaptureService";
//    private String NOTIFICATION_CHANNEL_DESC = "AudioCaptureService";
//    private int NOTIFICATION_ID = 1000;
//    private static final String NOTIFICATION_TICKER = "RecorderApp";
//    private int resultCode;
//    private Intent resultData;


    public static void notify(Context context, String message) {
        try {
            NotificationManager m_notificationManager = (NotificationManager)
                    context.getSystemService(Context.NOTIFICATION_SERVICE);


                    // 1
//                       // Intent notificationIntent = new Intent(this, ScreenCaptureServices.class);
//                       Intent notificationIntent = new Intent(this, AlertDetails.class);
//                        PendingIntent pendingIntent;
//                        if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.S) {
//                            pendingIntent = PendingIntent.getActivity(this, 0, notificationIntent, PendingIntent.FLAG_MUTABLE);
//                        } else {
//                            pendingIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);
//                        }



// 2

//                        // Create an Intent for the activity you want to start.
//                        Intent resultIntent = new Intent(this, ResultActivity.class);
//                        // Create the TaskStackBuilder and add the intent, which inflates the back
//                        // stack.
//                        TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
//                        stackBuilder.addNextIntentWithParentStack(resultIntent);
//                        // Get the PendingIntent containing the entire back stack.
//                        PendingIntent resultPendingIntent =
//                                stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT | PendingIntent.FLAG_IMMUTABLE);

//3
//                        Intent resultIntent = new Intent(this, ResultActivity.class);
//                        TaskStackBuilder stackBuilder = TaskStackBuilder.create(this);
//                        // Adds the back stack
//                        stackBuilder.addParentStack(ResultActivity.class);
//                        // Adds the Intent to the top of the stack
//                        stackBuilder.addNextIntent(resultIntent);
//                        // Gets a PendingIntent containing the entire back stack
//                        PendingIntent resultPendingIntent =
//                                stackBuilder.getPendingIntent(0, PendingIntent.FLAG_UPDATE_CURRENT);

//4

// Create an explicit intent for an Activity in your app.
//            <b>Intent intent = new Intent(this, AlertDetails.class);
//            intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_CLEAR_TASK);</b>
//            PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_IMMUTABLE);


            Notification.Builder m_builder;
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.O) {
                int importance = NotificationManager.IMPORTANCE_DEFAULT;
                NotificationChannel notificationChannel;
                notificationChannel = new NotificationChannel("Qt", "Qt Notifier", importance);
                m_notificationManager.createNotificationChannel(notificationChannel);
                m_builder = new Notification.Builder(context, notificationChannel.getId());
            } else {
                m_builder = new Notification.Builder(context);
            }

            Bitmap icon = BitmapFactory.decodeResource(context.getResources(), R.drawable.icon);
            m_builder.setSmallIcon(R.drawable.icon)
                    .setLargeIcon(icon)
                  //  .setContentTitle("A message from Qt!")
                    .setContentText(message)
                    .setPriority(Notification.PRIORITY_DEFAULT)
                    .setDefaults(Notification.DEFAULT_SOUND)
                  //  .setColor(Color.GREEN)
                     .setAutoCancel(true);
                  // Set the intent that fires when the user taps the notification.
              //      .setContentIntent(resultPendingIntent);

            m_notificationManager.notify(0, m_builder.build());
        } catch (Exception e) {
            e.printStackTrace();
        }

///////////
//    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
//    //Call Start foreground with notification
// //   Intent notificationIntent = new Intent(this, ScreenCaptureServices.class);
//    PendingIntent pendingIntent;
//    if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.S) {
//        pendingIntent = PendingIntent.getActivity(this, 0, notificationIntent, PendingIntent.FLAG_MUTABLE);
//    } else {
//        pendingIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);
//    }
//    NotificationCompat.Builder notificationBuilder = new NotificationCompat.Builder(this, NOTIFICATION_CHANNEL_ID)
//            .setLargeIcon(BitmapFactory.decodeResource(getResources(), R.drawable.ic_launcher_foreground))
//            .setSmallIcon(R.drawable.ic_launcher_foreground)
//            .setContentTitle("Starting Service")
//            .setContentText("Starting monitoring service")
//            .setTicker(NOTIFICATION_TICKER)
//            .setContentIntent(pendingIntent);
//    Notification notification = notificationBuilder.build();
//    NotificationChannel channel = new NotificationChannel(NOTIFICATION_CHANNEL_ID, NOTIFICATION_CHANNEL_NAME, NotificationManager.IMPORTANCE_DEFAULT);
//    channel.setDescription(NOTIFICATION_CHANNEL_DESC);
//    NotificationManager notificationManager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
//    notificationManager.createNotificationChannel(channel);
//    startForeground(NOTIFICATION_ID, notification);
//    //notificationManager.notify(NOTIFICATION_ID, notification);

//}

    }
}

