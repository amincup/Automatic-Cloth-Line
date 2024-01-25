package com.example.jemuran;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.ImageView;
import android.widget.TextView;

import com.firebase.client.DataSnapshot;
import com.firebase.client.Firebase;
import com.firebase.client.FirebaseError;
import com.firebase.client.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    //inisialisasi status cuaca
    private TextView weather;
    private TextView rain;
    private TextView jemuran;

    //refrence database koneksi langsung ke database firebase
    private Firebase nRef;
    private Firebase nRef1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        //baca komponen Monitoring
        weather = (TextView)findViewById(R.id.weather);
        rain = (TextView)findViewById(R.id.rain);
        jemuran = (TextView)findViewById(R.id.jemuran);

        //buka koneksi ke firebase
        nRef = new Firebase("https://jemuran-awikwok-default-rtdb.asia-southeast1.firebasedatabase.app/Monitoring/Cahaya");
        nRef1 = new Firebase("https://jemuran-awikwok-default-rtdb.asia-southeast1.firebasedatabase.app/Monitoring/Cuaca");
        //realtime process untuk ldr sensor
        nRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                //ambil nilai field value yg dari firebase
                int Cahaya = dataSnapshot.getValue(Integer.class);
                //pengondisian tampilan textview
                if (Cahaya > 100){
                    weather.setText("Siang");
                }else if (Cahaya  < 100){
                    weather.setText("Malam");
                    jemuran.setText("Terangkat");
                }else {
                }
            }

            @Override
            public void onCancelled(FirebaseError firebaseError) {
            }
        });

        //realtime process untuk raindrops sensor
        nRef1.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                //ambil nilai field value yg dari firebase
                int Cuaca = dataSnapshot.getValue(Integer.class);
                //pengondisian tampilan textview
                if (Cuaca > 0){
                    rain.setText("Sedang Tidak Hujan");
                    jemuran.setText("Terjemur");
                }else if (Cuaca  < 1){
                    rain.setText("Sedang Hujan");
                    jemuran.setText("Terangkat");
                }else {
                }
            }

            @Override
            public void onCancelled(FirebaseError firebaseError) {

            }
        });
    }
}