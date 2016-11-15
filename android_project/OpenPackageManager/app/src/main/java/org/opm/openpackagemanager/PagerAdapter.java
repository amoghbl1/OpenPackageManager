package org.opm.openpackagemanager;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;

/**
 * Created by bhavesh on 16/11/16.
 */


public class PagerAdapter extends FragmentStatePagerAdapter {

    int mNumOfTabs;

    public PagerAdapter(FragmentManager fm, int numTabs) {
        super(fm);
        this.mNumOfTabs = numTabs;
    }


    @Override
    public Fragment getItem(int position) {
        switch (position) {
            case 0:
                Home tab1 = new Home();
                return tab1;
            case 1:
                ListFragment tab2 = new ListFragment();
                return tab2;
            case 2:
                ServerListFragment tab3 = new ServerListFragment();
                return tab3;
            default:
                return null;
        }
    }

    @Override
    public int getCount() {
        return mNumOfTabs;
    }
}