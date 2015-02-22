/**
 * Created by alex on 7/16/14.
 */
package com.alexlee188.sgnurse;

public class job {
    private String job_id;
    private String job_date_time;
    private String post_district;
    private String job_details;

    public job(String job_id, String job_date_time, String post_district, String job_details){
        super();
        this.job_id = job_id;
        this.job_date_time = job_date_time;
        this.post_district = post_district;
        this.job_details = job_details;
    }
    public String get_job_id() { return job_id; }
    public String get_job_date_time(){
        return job_date_time;
    }
    public String get_post_district(){
        return post_district;
    }
    public String get_job_details(){
        return job_details;
    }
}
