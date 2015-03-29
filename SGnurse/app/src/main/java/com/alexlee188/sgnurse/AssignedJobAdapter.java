package com.alexlee188.sgnurse;

/**
 * Created by alex on 7/16/14.
 */
import java.util.ArrayList;
import android.content.Context;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

public class AssignedJobAdapter extends ArrayAdapter<job> {

    private final Context context;
    private final ArrayList<job> JobsArrayList;

    public AssignedJobAdapter(Context context, ArrayList<job> itemsArrayList) {

        super(context, R.layout.assigned_job_row, itemsArrayList);

        this.context = context;
        this.JobsArrayList = itemsArrayList;
    }

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {

        // 1. Create inflater
        LayoutInflater inflater = (LayoutInflater) context
                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);

        // 2. Get rowView from inflater
        View rowView = inflater.inflate(R.layout.assigned_job_row, parent, false);

        TextView job_heading = (TextView) rowView.findViewById(R.id.job_heading);
        TextView job_status = (TextView) rowView.findViewById(R.id.job_status);
        TextView job_date_timeView = (TextView) rowView.findViewById(R.id.job_date_time);
        TextView post_districtView = (TextView) rowView.findViewById(R.id.post_district);
        TextView job_detailView = (TextView) rowView.findViewById(R.id.job_details);

        job_heading.setText("JOB STATUS: ");
        String status = JobsArrayList.get(position).get_job_status();
        job_status.setText(status);
        //job_status.setTextColor(status.equalsIgnoreCase("requested") ? Color.parseColor("#009900") : Color.parseColor("#000000"));
        job_date_timeView.setText(JobsArrayList.get(position).get_job_date_time());
        post_districtView.setText(JobsArrayList.get(position).get_post_district());
        job_detailView.setText(JobsArrayList.get(position).get_job_details());

        // 5. return rowView
        return rowView;
    }
}