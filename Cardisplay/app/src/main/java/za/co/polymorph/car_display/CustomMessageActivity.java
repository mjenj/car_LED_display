package za.co.polymorph.car_display;

import android.os.Bundle;
import android.support.design.widget.TextInputEditText;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.WindowManager;

public class CustomMessageActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_custom_message);

        findViewById(R.id.bt_send).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SocketTask.send(
                        GlobalDataHolder.ipAddress,
                        GlobalDataHolder.port,
                        ((TextInputEditText)findViewById(R.id.et_message)).getText().toString(),
                        GlobalDataHolder.cycleColor,
                        GlobalDataHolder.red,
                        GlobalDataHolder.green,
                        GlobalDataHolder.blue);
            }
        });
    }

    @Override
    public void onBackPressed() {
        MenuActivity.startActivity(this);
    }
}
