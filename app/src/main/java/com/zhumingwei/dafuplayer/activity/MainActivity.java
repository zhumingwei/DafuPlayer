package com.zhumingwei.dafuplayer.activity;

import android.Manifest;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;
import android.view.View;


import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import com.zhumingwei.comic.medialibrary.engine.MediaScanEngine;
import com.zhumingwei.comic.medialibrary.listener.OnMediaSelectedListener;
import com.zhumingwei.comic.medialibrary.loader.impl.GlideMediaLoader;
import com.zhumingwei.comic.medialibrary.model.MimeType;
import com.zhumingwei.comic.utilslibrary.utils.PermissionUtils;
import com.zhumingwei.dafuplayer.R;

import java.util.List;

public class MainActivity extends AppCompatActivity {

    private static final int REQUEST_CODE = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        checkPermissions();
        findViewById(R.id.btn_video).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                scanMedia(false, false, true);
            }
        });
    }

    private void checkPermissions() {
        boolean cameraEnable = PermissionUtils.permissionChecking(this,
                Manifest.permission.CAMERA);
        boolean storageWriteEnable = PermissionUtils.permissionChecking(this,
                Manifest.permission.WRITE_EXTERNAL_STORAGE);
        boolean recordAudio = PermissionUtils.permissionChecking(this,
                Manifest.permission.RECORD_AUDIO);
        if (!cameraEnable || !storageWriteEnable || !recordAudio) {
            ActivityCompat.requestPermissions(this,
                    new String[] {
                            Manifest.permission.CAMERA,
                            Manifest.permission.WRITE_EXTERNAL_STORAGE,
                            Manifest.permission.RECORD_AUDIO
                    }, REQUEST_CODE);
        }
    }

    /**
     * 扫描媒体库
     * @param enableGif
     * @param enableImage
     * @param enableVideo
     */
    private void scanMedia(boolean enableGif, boolean enableImage, boolean enableVideo) {
        MediaScanEngine.from(this)
                .setMimeTypes(MimeType.ofAll())
                .ImageLoader(new GlideMediaLoader())
                .spanCount(4)
                .showCapture(false)
                .showImage(enableImage)
                .showVideo(enableVideo)
                .enableSelectGif(enableGif)
                .setMediaSelectedListener(new OnMediaSelectedListener() {
                    @Override
                    public void onSelected(List<Uri> uriList, List<String> pathList, boolean isVideo) {
                        if (isVideo) {
                            Intent intent = new Intent(MainActivity.this, AVMediaPlayerActivity.class);
                            intent.putExtra(AVMediaPlayerActivity.PATH, pathList.get(0));
                            startActivity(intent);
                        }
                    }
                })
                .scanMedia();
    }

}
