/**
 * Created by alex on 7/16/14.
 */
package com.alexlee188.sgnurse;

public class job {
    private String job_id;
    private String job_date_time;
    private String post_district;
    private String job_details;
    private String customer_name_1;
    private String customer_name_2;
    private String customer_addr_blk_no;
    private String customer_addr_street_1;
    private String customer_addr_street_2;
    private String customer_phone;
    private String customer_mobile;


    public job(String job_id, String job_date_time, String post_district, String job_details){
        super();
        this.job_id = job_id;
        this.job_date_time = job_date_time;
        this.post_district = post_district;
        this.job_details = job_details;
    }

    public job(String job_id, String job_date_time, String post_district, String job_details,
               String customer_name_1, String customer_name_2, String customer_addr_blk_no,
               String customer_addr_street_1, String customer_addr_street_2, String customer_phone,
               String customer_mobile){
        super();
        this.job_id = job_id;
        this.job_date_time = job_date_time;
        this.post_district = post_district;
        this.job_details = job_details;
        this.customer_name_1 = customer_name_1;
        this.customer_name_2 = customer_name_2;
        this.customer_addr_blk_no = customer_addr_blk_no;
        this.customer_addr_street_1 = customer_addr_street_1;
        this.customer_addr_street_2 = customer_addr_street_2;
        this.customer_phone = customer_phone;
        this.customer_mobile = customer_mobile;
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
    public String getCustomer_name_1(){ return customer_name_1;}
    public String getCustomer_name_2(){ return customer_name_2;}
    public String getCustomer_addr_blk_no(){ return customer_addr_blk_no;}
    public String getCustomer_addr_street_1(){return customer_addr_street_1;}
    public String getCustomer_addr_street_2(){return customer_addr_street_2;}
    public String getCustomer_phone(){return customer_phone;}
    public String getCustomer_mobile(){return customer_mobile;}
}
