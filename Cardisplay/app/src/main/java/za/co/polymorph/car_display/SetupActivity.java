package za.co.polymorph.car_display;

import android.os.Bundle;
import android.support.design.widget.TextInputEditText;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.WindowManager;

public class SetupActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                WindowManager.LayoutParams.FLAG_FULLSCREEN);
        setContentView(R.layout.activity_setup);

        ((TextInputEditText)findViewById(R.id.et_ip)).setText(GlobalDataHolder.ipAddress);
        ((TextInputEditText)findViewById(R.id.et_port)).setText(GlobalDataHolder.port);

        findViewById(R.id.bt_setup).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                GlobalDataHolder.ipAddress = ((TextInputEditText)findViewById(R.id.et_ip)).getText().toString();
                GlobalDataHolder.port = ((TextInputEditText)findViewById(R.id.et_port)).getText().toString();
                onBackPressed();
            }
        });
    }

    @Override
    public void onBackPressed() {
        MenuActivity.startActivity(this);
    }
}
