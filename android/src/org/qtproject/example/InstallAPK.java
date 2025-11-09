package org.qtproject.example;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Build;
import android.provider.Settings;
import android.support.v4.content.FileProvider;
import android.util.Log;
import org.qtproject.qt.android.QtNative;
import java.io.File;

public class InstallAPK {
    private static final String TAG = "InstallAPK";

    
    public static int execute(android.content.Context context, String apkAbsolutePath) {
        Activity activity = QtNative.activity();
        if (activity == null && context instanceof Activity) {
            activity = (Activity) context;
        }
        if (activity == null) {
            Log.e(TAG, "Activity is null");
            return -1;
        }

        if (apkAbsolutePath == null || apkAbsolutePath.isEmpty()) {
            Log.e(TAG, "APK path is empty");
            return -2;
        }

        File apkFile = new File(apkAbsolutePath);
        if (!apkFile.exists()) {
            Log.e(TAG, "APK not found: " + apkAbsolutePath);
            return -3;
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            boolean canInstall = activity.getPackageManager().canRequestPackageInstalls();
            if (!canInstall) {
                Intent permIntent = new Intent(Settings.ACTION_MANAGE_UNKNOWN_APP_SOURCES,
                        Uri.parse("package:" + activity.getPackageName()));
                permIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                activity.startActivity(permIntent);
                Log.w(TAG, "Requesting 'Install unknown apps' permission. User must grant and retry.");
                return -4;
            }
        }

        try {
            // authority ДОЛЖЕН совпадать с манифестом: org.qtproject.example.fileprovider
            Uri contentUri = FileProvider.getUriForFile(
                    activity,
                    "org.qtproject.example.fileprovider",
                    apkFile
            );

            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.setDataAndType(contentUri, "application/vnd.android.package-archive");
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);

            activity.startActivity(intent);
            return 0;
        } catch (Exception e) {
            Log.e(TAG, "Failed to start installer", e);
            return -5;
        }
    }
}
