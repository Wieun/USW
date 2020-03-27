using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Web.Script.Serialization;

namespace _16059980_Event_Drive_CW_2
{
    // Delegate used to register to event on RemoteFlightController
    // (see class below form).
    public delegate void Control(RemoteFlightController.ControlsUpdate control);
    public delegate void Warning(string warning);
    public delegate void Telemetry(RemoteFlightController.TelemetryUpdate telemetry);

    public partial class frmRemoteFlightController : Form
    {
        // myRemoteFlightController class defined below. Triggers event each
        // time a number event is invoked.
        RemoteFlightController myRemoteFlightController = new RemoteFlightController();

        public frmRemoteFlightController()
        {
            InitializeComponent();

            // Register delegates to event on RemoteFlightController.
            myRemoteFlightController.OnTelemetryUpdate += new Telemetry(telemetryUpdate);
            myRemoteFlightController.OnWarningCode += new Warning(displayWarning);
            myRemoteFlightController.OnControlUpdate += new Control(controlUpdate);

            // Populate my IP Details.
            string hostName = Dns.GetHostName();
            IPHostEntry ipInfo = Dns.GetHostEntry(hostName);

            foreach (IPAddress ip in ipInfo.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    cmbIPAddress.Items.Add(ip);
                }
            }

            if (cmbIPAddress.Items.Count > 0)
            {
                cmbIPAddress.SelectedIndex = 0;
            }
            else
            {
                throw new Exception("No valid IP Addresses Located.");
            }

            dgvTelematryUpdates.ColumnCount = 7;
            dgvTelematryUpdates.RowCount = 6;
            dgvTelematryUpdates.Columns[0].Name = "Altitude";
            dgvTelematryUpdates.Columns[1].Name = "Speed";
            dgvTelematryUpdates.Columns[2].Name = "Pitch";
            dgvTelematryUpdates.Columns[3].Name = "Vertical Speed";
            dgvTelematryUpdates.Columns[4].Name = "Throttle";
            dgvTelematryUpdates.Columns[5].Name = "Elevator Pitch";
            dgvTelematryUpdates.Columns[6].Name = "Warning";
        }

        public void trkControls_Scroll(object sender, EventArgs e)
        {
            double throttle = trkThrottle.Value; // multiply track bar value by 5 to make it correct since track bar is from 0-20 e.g. 20*5 = 100
            lblThrottleCurrent.Text = throttle.ToString() + '%'; //diplay throttle for user to see

            double pitch;
            pitch = (double)trkElevatorPitch.Value / 10; //divide track bar value by 10 to make it decimal e.g. 50/10 = 5.0
            lblElevatorPitchCurrent.Text = string.Format("{0:F1}", pitch); //display Elevator pitch for user to see formats it to 1 decimal place

            myRemoteFlightController.updateControls(throttle, pitch); //pass the throttle to updateThrottleControls method to update the simulator throttle
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            // send the connection info (IP & port) to the connectToFlightSimulator method 
            myRemoteFlightController.connectToFlightSimulator(cmbIPAddress.SelectedItem.ToString(), int.Parse(txtPort.Text));
        }

        public void telemetryUpdate(RemoteFlightController.TelemetryUpdate message)
        {
            // Check we are on the correct thread, if not and invoke is
            // require, invoke this method is called on correct thread.
            if (this.InvokeRequired)
            {
                this.Invoke(new Telemetry(telemetryUpdate), new object[] { message });
            }
            else
            {
                for (int i = 0; i < dgvTelematryUpdates.Rows.Count - 1; i++)
                {
                    //goes through the data grid moving each row down by one and replacing it with newer data
                    dgvTelematryUpdates.Rows[i + 1].Cells[0].Value = dgvTelematryUpdates.Rows[i].Cells[0].Value;
                    dgvTelematryUpdates.Rows[i + 1].Cells[1].Value = dgvTelematryUpdates.Rows[i].Cells[1].Value;
                    dgvTelematryUpdates.Rows[i + 1].Cells[2].Value = dgvTelematryUpdates.Rows[i].Cells[2].Value;
                    dgvTelematryUpdates.Rows[i + 1].Cells[3].Value = dgvTelematryUpdates.Rows[i].Cells[3].Value;
                    dgvTelematryUpdates.Rows[i + 1].Cells[4].Value = dgvTelematryUpdates.Rows[i].Cells[4].Value;
                    dgvTelematryUpdates.Rows[i + 1].Cells[5].Value = dgvTelematryUpdates.Rows[i].Cells[5].Value;
                    dgvTelematryUpdates.Rows[i + 1].Cells[6].Value = dgvTelematryUpdates.Rows[i].Cells[6].Value;
                }
                //add the new data to the data grid(add to the first row)
                dgvTelematryUpdates.Rows[0].Cells[0].Value = string.Format("{0:F2}", message.Altitude); //add the new data (formats it to 2dp)
                dgvTelematryUpdates.Rows[0].Cells[1].Value = string.Format("{0:F2}", message.Speed); //add the new data (formats it to 2dp)
                dgvTelematryUpdates.Rows[0].Cells[2].Value = string.Format("{0:F2}", message.Pitch); //add the new data (formats it to 2dp)
                dgvTelematryUpdates.Rows[0].Cells[3].Value = string.Format("{0:F2}", message.VerticalSpeed); //add the new data (formats it to 2dp)
                dgvTelematryUpdates.Rows[0].Cells[4].Value = message.Throttle; //add the new data 
                dgvTelematryUpdates.Rows[0].Cells[5].Value = message.ElevatorPitch; //add the new data 
                dgvTelematryUpdates.Rows[0].Cells[6].Value = message.WarningCode; //add the new data 

                //updates the track bars and labels to display the new track bar values received from the flight simulator
                trkElevatorPitch.Value = (int)(message.ElevatorPitch * 10);
                lblElevatorPitchCurrent.Text = message.ElevatorPitch.ToString();
                trkThrottle.Value = (int)(message.Throttle);
                lblThrottleCurrent.Text = message.Throttle.ToString() + '%';
            }
        }

        public void controlUpdate(RemoteFlightController.ControlsUpdate message)
        {
            // Check we are on the correct thread, if not and invoke is
            // require, invoke this method is called on correct thread.
            if (this.InvokeRequired)
            {
                this.Invoke(new Control(controlUpdate), new object[] { message });
            }
            else
            {
                string Throttle = "Throttle = " + message.Throttle.ToString();
                string ElevatorPitch = "   Elevator Pitch = " + message.ElevatorPitch.ToString();
                string update = Throttle + ElevatorPitch;
                txtControlUpdate.AppendText(update);
                txtControlUpdate.AppendText(Environment.NewLine);
            }
        }

        public void displayWarning(string warning) //checks the warning code and displays the corresponding message if if warning code is 0 then no warning
        {
            // Check we are on the correct thread, if not and invoke is
            // require, invoke this method is called on correct thread.
            if (this.InvokeRequired)
            {
                this.Invoke(new Warning(displayWarning), new object[] { warning });
            }
            else
            {
                lblWarning.Text = warning;
            }
        }
    }

    public class RemoteFlightController
    {
        // events
        public event Control OnControlUpdate;
        public event Warning OnWarningCode;
        public event Telemetry OnTelemetryUpdate;

        JavaScriptSerializer serializer = new JavaScriptSerializer(); //Java Script Serializer

        TcpClient m_client = null;

        Thread listeningThread = null;

        public struct ControlsUpdate
        {
            public double Throttle;         //Current throttle setting as a percentage          (i.e. 0% no throttle, 100% full throttle).
            public double ElevatorPitch;    //Current Elevator Pitch in degrees. Positive creates upwards lift, negative downwards.
        }
        ControlsUpdate Controlsdata = new ControlsUpdate();

        public struct TelemetryUpdate
        {
            public double Altitude;      //Altitude in ft.
            public double Speed;         //Plane's speed in Knts.
            public double Pitch;         //Plane's pitch in degrees relative to horizon. Positive is planes pointing upwards, negative plane points downwards;
            public double VerticalSpeed; //Plane's vertical speed in Feet per minute.

            public double Throttle;      //Current throttle setting as a percentage (i.e. 0% no throttle, 100% full throttle).
            public double ElevatorPitch; //Current Elevator Pitch in degrees. Positive creates upwards lift, negative downwards.

            public int WarningCode;   //Warning code: 0 - No Warnings; 1 -  Too Low (less than 1000ft); 2 - Stall.
        }

        public void updateControls(double throttle, double elevator)
        {
            Controlsdata.Throttle = throttle; // sets Throttle to the throttle received from trkThrottle
            Controlsdata.ElevatorPitch = elevator;
        }


        public void update()
        {
            NetworkStream stream = m_client.GetStream(); //network stream object to receive data

            while (true)
            {
                string jsonString = serializer.Serialize(Controlsdata); // string we want to send

                byte[] rawData = Encoding.ASCII.GetBytes(jsonString); //convert jsonString to bytes

                stream.Write(rawData, 0, rawData.Length);

                OnControlUpdate?.Invoke(Controlsdata);

                Thread.Sleep(120);
            }
        }

        public void Listen()
        {
            NetworkStream stream = m_client.GetStream(); //network stream object to receive data

            byte[] buffer = new byte[256]; // buffer to write into
            
            while (true)
            {
                try
                {
                    int num_bytes = stream.Read(buffer, 0, 256); //256 is the max number of bytes we're willing to accept

                    string message = Encoding.ASCII.GetString(buffer, 0, num_bytes); //decodes the data only up to bytes received

                    TelemetryUpdate obj = serializer.Deserialize<TelemetryUpdate>(message); // decodes the string(message) 

                    //the events will only invoke if the event is not null

                    OnTelemetryUpdate?.Invoke(obj);

                    if (obj.WarningCode == 1)
                    {
                        OnWarningCode?.Invoke("Low altitude: fly back above 1000ft");
                    }
                    else if (obj.WarningCode == 0)
                    {
                        OnWarningCode?.Invoke("");
                    }
                    else if (obj.WarningCode == 2)
                    {
                        OnWarningCode?.Invoke("Stall risk: increase the speed or drop the nose to lower the elevator. ");
                    }

                    double throttle = obj.Throttle;
                    double elevator = obj.ElevatorPitch;
                    updateControls(throttle, elevator);
                }
                catch (Exception)
                {

                }
            }
        }

        public void connectToFlightSimulator(string IP, int port)
        {
            m_client = new TcpClient();
            IPAddress ip = IPAddress.Parse(IP); //converts string to IP Address
            m_client.Connect(IP, port); //connects to the specified IP Address and port
            MessageBox.Show("Connected to: " + IP);

            Thread listenerThread = new Thread(new ThreadStart(Listen)); //listeningThread invokes the Listen method 6
            listenerThread.Start(); // listeningThread starts listening for messages
            listenerThread = listeningThread;

            Thread updaterThread = new Thread(new ThreadStart(update)); //listeningThread invokes the Listen method 6
            updaterThread.Start(); // listeningThread starts listening for messages
            updaterThread = listeningThread;
        }

    }

}

