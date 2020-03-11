package za.co.polymorph.car_display;

import android.os.AsyncTask;
import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;

public class SocketTask extends AsyncTask<String, String, Void> {

    private static final String TAG_STRING = "SocketTask";
    private Socket socket;
    private PrintWriter out;
    private BufferedReader in;

    @Override
    protected Void doInBackground(String... params) {
        try {
            String ip = params[0];
            int port = Integer.parseInt(params[1]);
            String message = params[2];
            socket = new Socket(ip, port);
            if (socket.isConnected()) {
                out = new PrintWriter(new BufferedWriter(
                        new OutputStreamWriter(socket.getOutputStream())), true);
                out.println(message);
                out.close();
                socket.close();

            } else {
                Log.i(TAG_STRING, "Socket is not connected");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        return null;
    }

    public static void send(String ip, String port, String message, String cycleColor, String red, String blue, String green) {
        SocketTask task = new SocketTask();
        task.execute(ip, port, buildPlainText(message, cycleColor, red, blue, green));
    }

    private static String buildMessage(String message, String cycleColor, String red, String blue, String green) {
        return "[" + message + "," + cycleColor + "," + red + "," + blue + "," + green + "]";
    }

    private static String buildPlainText(String message, String cycleColor, String red, String blue, String green) {
        return message;
    }
}
