package com.zhumingwei.comic.medialibrary.adapter;


import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentPagerAdapter;

import com.zhumingwei.comic.medialibrary.fragment.PreviewFragment;
import com.zhumingwei.comic.medialibrary.model.MediaItem;

import java.util.ArrayList;
import java.util.List;

/**
 * 预览页面适配器
 */
public class PreviewPagerAdapter extends FragmentPagerAdapter {

    private ArrayList<MediaItem> mMediaItems = new ArrayList<>();

    public PreviewPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        return PreviewFragment.newInstance(mMediaItems.get(position));
    }

    @Override
    public int getCount() {
        return mMediaItems.size();
    }

    /**
     * 获取媒体对象
     * @param position
     * @return
     */
    public MediaItem getMediaItem(int position) {
        return mMediaItems.get(position);
    }

    /**
     * 添加媒体对象
     * @param items
     */
    public void addMediaItems(List<MediaItem> items) {
        mMediaItems.addAll(items);
    }

}
