package org.opm.openpackagemanager;

import android.content.Context;
import android.support.v7.widget.CardView;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import java.util.List;

/**
 * Created by bhavesh on 16/11/16.
 */

public class PackageAdapter extends RecyclerView.Adapter<PackageAdapter.MyViewHolder> {

    private Context mContext;
    private List<Package> packageList;
    private CardView card_view;

    public class MyViewHolder extends RecyclerView.ViewHolder {
        public TextView title, version;

        public MyViewHolder(View view) {
            super(view);
            card_view = (CardView)view.findViewById(R.id.list_cards);
            title = (TextView) view.findViewById(R.id.package_name);
            version = (TextView) view.findViewById(R.id.package_version);
        }
    }


    public PackageAdapter(Context mContext, List<Package> packageList) {
        this.mContext = mContext;
        this.packageList = packageList;
    }

    @Override
    public MyViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.list_cards, parent, false);

        return new MyViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(final MyViewHolder holder, int position) {
        Package album = packageList.get(position);
        holder.title.setText(album.getName());
        holder.version.setText("Version: " + album.getVersion());


    }

    @Override
    public int getItemCount() {
        return packageList.size();
    }
}
