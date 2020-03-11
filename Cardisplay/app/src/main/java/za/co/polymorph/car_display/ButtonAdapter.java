package za.co.polymorph.car_display;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;

import java.util.ArrayList;

public class ButtonAdapter extends RecyclerView.Adapter<RecyclerView.ViewHolder> {

    private String[] standardButtons = new String[]{"You suck", "Whohoo!", "It's a joyride!"};
    private Context context;

    private ArrayList<RowHolder> rowHolders;

    public ButtonAdapter(Context context) {
        this.context = context;
        rowHolders = new ArrayList<>();
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_button, parent, false);
        RowHolder row = new RowHolder(view, context, this);
        return row;
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, final int position) {
        RowHolder reasonRow = (RowHolder) holder;
        reasonRow.setText(standardButtons[position]);
        rowHolders.add(reasonRow);
    }

    @Override
    public int getItemCount() {
        return standardButtons.length;
    }

    class RowHolder extends RecyclerView.ViewHolder {

        private final Button button;
        private Context context;
        private ButtonAdapter adapter;

        public RowHolder(View view, final Context context, ButtonAdapter adapter) {
            super(view);
            button = (Button) view.findViewById(R.id.button);
            button.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    SocketTask.send(
                            GlobalDataHolder.ipAddress,
                            GlobalDataHolder.port,
                            button.getText().toString(),
                            GlobalDataHolder.cycleColor,
                            GlobalDataHolder.red,
                            GlobalDataHolder.green,
                            GlobalDataHolder.blue);
                }
            });
            this.context = context;
            this.adapter = adapter;
        }

        public void setText(String name) {
            this.button.setText(name);
        }
    }
}

