
package android.src.InstallAPK;
import org.qtproject.qt.android.QtNative;
import java.lang.String;
import java.io.File;
import android.content.Intent;
import android.util.Log;
import android.net.Uri;
import android.content.ContentValues;
import android.content.Context;
import android.support.v4.content.FileProvider;
import android.app.Activity;
import android.os.Bundle;


public class InstallAPK extends Activity
{
    protected InstallAPK()
    {
    }

    private  Context context = this.context;

    public  int installApp(String appPackageName) {

         if (QtNative.activity() == null)
            return -1;
         try {
                /*вставлять сюда*/

             File file = new File("/storage/emulated/0/Download/droid_stick.apk");
             Uri fileUri = FileProvider.getUriForFile(context,"org.qtproject.install.fileProvider", file);                                       //моя строка
         //    Uri fileUri = FileProvider.getUriForFile(activity.getBaseContext(), activity.getApplicationContext().getPackageName() + ".provider", file);  //была строка
             Intent intent = new Intent(Intent.ACTION_VIEW, fileUri);
             intent.putExtra(Intent.EXTRA_NOT_UNKNOWN_SOURCE, true);
             intent.setDataAndType(fileUri, "application/vnd.android" + ".package-archive");
             intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
             intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
             startActivity(intent);
            /***********************/

             return 0;
         } catch (android.content.ActivityNotFoundException anfe) {
             return -3;
         }
     }
}


/*
//как было
public  int installApp(String appPackageName) {
        if (QtNative.activity() == null)
            return -1;
        try {
            File apk = new File(appPackageName);
            Intent intent = new Intent(Intent.ACTION_VIEW);
            intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            Uri uri = FileProvider.getUriForFile(context,"org.qtproject.install.fileProvider", apk);
            intent.setDataAndType(uri,"application/vnd.android.package-archive");

            QtNative.activity().startActivity(intent);
            return 0;
        } catch (android.content.ActivityNotFoundException anfe) {
            return -3;
        }
    }
*/


/*
//второй вариант кода
File directory = context.getExternalFilesDir(null);
File file = new File("/storage/emulated/0/Download/droid_stick.apk");
Uri fileUri = Uri.fromFile(file);

    fileUri = FileProvider.getUriForFile(context, context.getPackageName(),
            file);

Intent intent = new Intent(Intent.ACTION_VIEW, fileUri);
intent.putExtra(Intent.EXTRA_NOT_UNKNOWN_SOURCE, true);
intent.setDataAndType(fileUri, "application/vnd.android" + ".package-archive");
intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
context.startActivity(intent);
//     activity.finish();
*/

/*
//третий вариант
Uri uri;
File file = new File("/storage/emulated/0/Download/droid_stick.apk");
Uri fileUri = FileProvider.getUriForFile(activity.getBaseContext(), activity.getApplicationContext().getPackageName() + ".provider", file);
Intent intent = new Intent(Intent.ACTION_VIEW, fileUri);
intent.putExtra(Intent.EXTRA_NOT_UNKNOWN_SOURCE, true);
intent.setDataAndType(fileUri, "application/vnd.android" + ".package-archive");
intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK);
intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
startActivity(intent);
*/

/*
//4
Uri uri;
File file = new File("/storage/emulated/0/Download/droid_stick.apk");
Intent intent1 = new Intent(Intent.ACTION_INSTALL_PACKAGE);
//   uri = FileProvider.getUriForFile(context.getApplicationContext(), BuildConfig.APPLICATION_ID + ".provider", file);     //была строка
uri = FileProvider.getUriForFile(context,"org.qtproject.install.fileProvider", file);                                       //моя строка
context.grantUriPermission("com.abcd.xyz", uri, Intent.FLAG_GRANT_READ_URI_PERMISSION);
context.grantUriPermission("com.abcd.xyz", uri, Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
intent1.setDataAndType(uri,
        "application/*");
intent1.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
intent1.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
intent1.addFlags(Intent.FLAG_GRANT_WRITE_URI_PERMISSION);
startActivity(intent1);
*/

/*
//не испытано
public class InstallManagerApk extends AppCompatActivity {

   static final String NAME_APK_FILE = "some.apk";
   public static final int REQUEST_INSTALL = 0;

    @Override
   protected void onCreate(@Nullable Bundle savedInstanceState) {
       super.onCreate(savedInstanceState);

       // required permission:
       // android.Manifest.permission.WRITE_EXTERNAL_STORAGE
       // android.Manifest.permission.READ_EXTERNAL_STORAGE

       installApk();

   }




    // Install APK File

   private void installApk() {

       try {

           File filePath = Environment.getExternalStorageDirectory();// path to file apk
           File file = new File(filePath, LoadManagerApkFile.NAME_APK_FILE);

           Uri uri = getApkUri( file.getPath() ); // get Uri for  each SDK Android

           Intent intent = new Intent(Intent.ACTION_INSTALL_PACKAGE);
           intent.setData( uri );
           intent.setFlags( Intent.FLAG_GRANT_READ_URI_PERMISSION | Intent.FLAG_ACTIVITY_NEW_TASK );
           intent.putExtra(Intent.EXTRA_NOT_UNKNOWN_SOURCE, true);
           intent.putExtra(Intent.EXTRA_RETURN_RESULT, true);
           intent.putExtra(Intent.EXTRA_INSTALLER_PACKAGE_NAME, getApplicationInfo().packageName);

           if ( getPackageManager().queryIntentActivities(intent, 0 ) != null ) {// checked on start Activity

               startActivityForResult(intent, REQUEST_INSTALL);

           } else {
               throw new Exception("don`t start Activity.");
           }

       } catch ( Exception e ) {

           Log.i(TAG + ":InstallApk", "Failed installl APK file", e);
           Toast.makeText(getApplicationContext(), e.getMessage(), Toast.LENGTH_LONG)
               .show();

       }

   }


   // Returns a Uri pointing to the APK to install.

   private Uri getApkUri(String path) {

       // Before N, a MODE_WORLD_READABLE file could be passed via the ACTION_INSTALL_PACKAGE
       // Intent. Since N, MODE_WORLD_READABLE files are forbidden, and a FileProvider is
       // recommended.
       boolean useFileProvider = Build.VERSION.SDK_INT >= Build.VERSION_CODES.N;

       String tempFilename = "tmp.apk";
       byte[] buffer = new byte[16384];
       int fileMode = useFileProvider ? Context.MODE_PRIVATE : Context.MODE_WORLD_READABLE;
       try (InputStream is = new FileInputStream(new File(path));
            FileOutputStream fout = openFileOutput(tempFilename, fileMode)) {

           int n;
           while ((n = is.read(buffer)) >= 0) {
               fout.write(buffer, 0, n);
           }

       } catch (IOException e) {
           Log.i(TAG + ":getApkUri", "Failed to write temporary APK file", e);
       }

       if (useFileProvider) {

           File toInstall = new File(this.getFilesDir(), tempFilename);
           return FileProvider.getUriForFile(this,  BuildConfig.APPLICATION_ID, toInstall);

       } else {

           return Uri.fromFile(getFileStreamPath(tempFilename));

       }

   }


    // Listener event on installation APK file

   @Override
   protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
       super.onActivityResult(requestCode, resultCode, data);

       if(requestCode == REQUEST_INSTALL) {

           if (resultCode == Activity.RESULT_OK) {
               Toast.makeText(this,"Install succeeded!", Toast.LENGTH_SHORT).show();
           } else if (resultCode == Activity.RESULT_CANCELED) {
               Toast.makeText(this,"Install canceled!", Toast.LENGTH_SHORT).show();
           } else {
               Toast.makeText(this,"Install Failed!", Toast.LENGTH_SHORT).show();
           }

       }

   }



}
*/
