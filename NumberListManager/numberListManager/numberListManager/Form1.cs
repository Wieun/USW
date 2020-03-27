using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace numberListManager
{
    public partial class Form1 : Form
    {
        int maxNum = 30;
        int probe = 0;

        public Form1()
        {
            InitializeComponent();
            statistics();
        }

        private void btnInitialise_Click(object sender, EventArgs e)
        {
            if (lstNumberList.Items.Count < maxNum)
            {
                Random rnd = new Random();
                int num = rnd.Next(0, 101);
                if (lstNumberList.Items.Count == 0)
                { lstNumberList.Items.Add(num); }
                do
                {
                    num = rnd.Next(0, 101);
                    if (linearSearch(num) == -1) { lstNumberList.Items.Add(num); }
                }
                while (lstNumberList.Items.Count < maxNum);
            }
            if (optSorted.Checked == true) { bubbleSort(); }
            buttonManager();
            statistics();
        }

        private void statistics()
        {
            if (lstNumberList.Items.Count == 0)
            {
                txtNoCount.Text = "0";
                txtFirstEntry.Text = "n/a";
                txtLastEntry.Text = "n/a";
                txtMax.Text = "n/a";
                txtMin.Text = "n/a";
                txtNumbersLeft.Text = "30";
            }
            else
            {
                int largestNumber = Convert.ToInt32(lstNumberList.Items[0]), numbersLeft = 0, smallestNumber = Convert.ToInt32(lstNumberList.Items[0]), firstEntry = 0, lastEntry = 0;

                txtNoCount.Text = lstNumberList.Items.Count.ToString();

                firstEntry = Convert.ToInt32(lstNumberList.Items[0]);
                txtFirstEntry.Text = firstEntry.ToString();

                lastEntry = Convert.ToInt32(lstNumberList.Items[lstNumberList.Items.Count - 1]);
                txtLastEntry.Text = lastEntry.ToString();

                for (int i = 0; i < lstNumberList.Items.Count; i++)
                {
                    if (largestNumber < Convert.ToInt32(lstNumberList.Items[i]))
                    {
                        largestNumber = Convert.ToInt32(lstNumberList.Items[i]);
                    }
                    if (smallestNumber > Convert.ToInt32(lstNumberList.Items[i]))
                    {
                        smallestNumber = Convert.ToInt32(lstNumberList.Items[i]);
                    }
                }
                txtMax.Text = largestNumber.ToString();
                txtMin.Text = smallestNumber.ToString();
                numbersLeft = maxNum - Convert.ToInt32(lstNumberList.Items.Count);
                txtNumbersLeft.Text = numbersLeft.ToString();
            }
        }

        private void optSorted_Click(object sender, EventArgs e)
        {
            bubbleSort();
            buttonManager();
            statistics();
        }

        private void bubbleSort()
        {
            int lastSwap = lstNumberList.Items.Count - 1;
            for (int i = 1; i < lstNumberList.Items.Count; i++)
            {
                bool sorted = true;
                int currentSwap = -1;
                for (int valuecount = 0; valuecount < lastSwap; valuecount++)
                {
                    if (Convert.ToInt32(lstNumberList.Items[valuecount]) > Convert.ToInt32(lstNumberList.Items[valuecount + 1]))
                    {
                        int temp = Convert.ToInt32(lstNumberList.Items[valuecount]);
                        lstNumberList.Items[valuecount] = lstNumberList.Items[valuecount + 1];
                        lstNumberList.Items[valuecount + 1] = temp;
                        sorted = false;
                        currentSwap = valuecount;
                    }
                }
                if (sorted) return;
                lastSwap = currentSwap;
            }
        }

        private void optUnsorted_Click(object sender, EventArgs e)
        {
            buttonManager();
        }

        private void optBinarySearch_Click(object sender, EventArgs e)
        {

        }

        private int binarySearch(int find)
        {
            int mid = 0, first = 0, last = lstNumberList.Items.Count - 1;
            probe = 0;
            bool found = false;

            while (!found && first <= last)
            {
                probe++;
                mid = (first + last) / 2;

                if (find == Convert.ToInt32(lstNumberList.Items[mid]))


                    return mid;

                else
                {
                    if (find > Convert.ToInt32(lstNumberList.Items[mid]))
                    {
                        first = mid + 1;
                    }

                    if (find < Convert.ToInt32(lstNumberList.Items[mid]))
                    {
                        last = mid - 1;
                    }
                }
            }
            return -1;
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            lstNumberList.Items.Clear();
            buttonManager();
            statistics();
        }

        private void btnAdd_Click(object sender, EventArgs e)
        {
            int num = int.Parse(txtInsertNumber.Text);
            if (num > -1 && num < 101)
            {
                if (lstNumberList.Items.Count < maxNum)
                {
                    if (lstNumberList.Items.Count == 0) { lstNumberList.Items.Add(num); }
                    else if (linearSearch(int.Parse(txtInsertNumber.Text)) != -1)
                    { MessageBox.Show(txtInsertNumber.Text + " is already in the list!", "Action not allowed!"); }
                    else { lstNumberList.Items.Add(num); }
                    if (optSorted.Checked == true && lstNumberList.Items.Count > 1)
                    {
                        int lastnum = lstNumberList.Items.Count - 1, temp = 0, numcCount = lstNumberList.Items.Count - 2;
                        bool inserted = false;
                        while (inserted == false)
                        {
                            while (Convert.ToInt32(lstNumberList.Items[lastnum]) < Convert.ToInt32(lstNumberList.Items[numcCount]))
                            {
                                temp = Convert.ToInt32(lstNumberList.Items[numcCount]);
                                lstNumberList.Items[numcCount] = Convert.ToInt32(lstNumberList.Items[lastnum]);
                                lstNumberList.Items[lastnum] = temp;
                                if (lstNumberList.Items.Count == 2) { break; }
                                lastnum--;
                                numcCount--;
                                if (numcCount == -1)
                                {
                                    break;
                                }
                            }
                            inserted = true;
                        }
                    }
                }
            }
            else { MessageBox.Show("Number must be 0 - 100 ", "Action not allowed!"); }
            txtInsertNumber.Text = "";
            buttonManager();
            txtInsertNumber.Focus();
            statistics();
        }

        private void optLinearSearch_Click(object sender, EventArgs e)
        {

        }

        private int linearSearch(int find)
        {
            int valuecount = 0;
            probe = 0;
            while (valuecount < lstNumberList.Items.Count)
            {
                probe++;
                if (Convert.ToInt32(lstNumberList.Items[valuecount]) == find)
                {
                    return valuecount;
                }
                valuecount++;
            }
            return -1;
        }

        private void btnShuffle_Click(object sender, EventArgs e)
        {
            optUnsorted.Checked = true;
            ListBox.ObjectCollection list = lstNumberList.Items;
            Random rnd = new Random();
            int NoOfNumbers = list.Count;
            lstNumberList.BeginUpdate();
            while (NoOfNumbers > 1)
            {
                NoOfNumbers--;
                int k = rnd.Next(NoOfNumbers + 1);
                object value = list[k];
                list[k] = list[NoOfNumbers];
                list[NoOfNumbers] = value;
            }
            lstNumberList.EndUpdate();
            lstNumberList.Invalidate();
            buttonManager();
            statistics();
        }

        private void btnDelete_Click(object sender, EventArgs e)
        {
            if (lstNumberList.SelectedIndex == -1)
            {
                int VisibleTime = 1000;
                toolTip1.Show("select what you want to delete", btnDelete, 0, 0, VisibleTime);
            }
            delete();
            buttonManager();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            pictureBox1.AllowDrop = true;
        }

        private void lstNumberList_MouseDown(object sender, MouseEventArgs e)
        {
            if (lstNumberList.Items.Count > 0)
            {
                lstNumberList.DoDragDrop(lstNumberList.Items, DragDropEffects.Move);
            }
        }

        private void pictureBox1_DragEnter(object sender, DragEventArgs e)
        {
            e.Effect = DragDropEffects.Move;
        }

        private void pictureBox1_DragDrop(object sender, DragEventArgs e)
        {
            delete();
        }

        private void delete()
        {
            int valuecount = 0;
            int indx = lstNumberList.SelectedIndex;
            if (indx == -1) return;
            while (valuecount < lstNumberList.Items.Count - 1)
            {
                if (indx == valuecount)
                {
                    int temp = Convert.ToInt32(lstNumberList.Items[valuecount]);
                    lstNumberList.Items[valuecount] = lstNumberList.Items[valuecount + 1];
                    lstNumberList.Items[valuecount + 1] = temp;
                    indx++;
                }
                valuecount++;
            }
            lstNumberList.Items.RemoveAt(lstNumberList.Items.Count - 1);

            statistics(); buttonManager();
        }

        private void buttonManager()
        {
            if (lstNumberList.Items.Count == 0)
            {
                btnSearch.Enabled = false;
                optLinearSearch.Enabled = false;
                optBinarySearch.Enabled = false;
                btnClear.Enabled = false;
                btnDelete.Enabled = false;
                btnShuffle.Enabled = false;
                btnInitialise.Enabled = true;
            }

            if (lstNumberList.Items.Count > 0)
            {
                btnClear.Enabled = true;
                btnDelete.Enabled = true;
                btnInitialise.Enabled = true;
                optLinearSearch.Enabled = true;
            }
            if (lstNumberList.Items.Count == maxNum)
            {
                btnAdd.Enabled = false;
                btnInitialise.Enabled = false;
            }
            if (lstNumberList.Items.Count > 1)
            {
                btnShuffle.Enabled = true;
            }
            if (optUnsorted.Checked == true)
            {
                optBinarySearch.Enabled = false;
                optLinearSearch.Checked = true;

            }
            if (optSorted.Checked == true && lstNumberList.Items.Count > 0)
            {
                optLinearSearch.Enabled = true;
                optBinarySearch.Enabled = true;

            }
            if (lstNumberList.Items.Count == 1)
            {
                btnShuffle.Enabled = false;
            }
            txtInsertNumber.Text = "";
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {

            int find = int.Parse(txtInsertNumber.Text);
            if (optBinarySearch.Checked == true)
            {
                int binReturnVal = binarySearch(find);
                if (optBinarySearch.Checked == true)
                {
                    if (binReturnVal != -1)
                    {
                        MessageBox.Show(txtInsertNumber.Text + " found at position " + binReturnVal);
                        lstNumberList.SetSelected(binReturnVal, true);
                    }
                    else
                    { MessageBox.Show(txtInsertNumber.Text + " not found"); }
                }
                MessageBox.Show("the probe is " + probe);
            }

            if (optLinearSearch.Checked == true)
            {
                int linReturnVal = linearSearch(find);
                if (lstNumberList.Items.Count > 0)
                {
                    if (linearSearch(int.Parse(txtInsertNumber.Text)) != -1)
                    {
                        MessageBox.Show(txtInsertNumber.Text + " has been found at position " + linearSearch(int.Parse(txtInsertNumber.Text)));
                        lstNumberList.SetSelected(linReturnVal, true);
                    }
                    else
                    { MessageBox.Show(txtInsertNumber.Text + " has not been found"); }
                }
                MessageBox.Show("the probe is " + probe);
            }
            buttonManager();
            statistics();
        }

        private void txtInsertNumber_KeyPress(object sender, KeyPressEventArgs e)
        {
            txtInsertNumber.MaxLength = 3;
            if (!char.IsControl(e.KeyChar) && !char.IsDigit(e.KeyChar))
            {
                e.Handled = true;
                int VisibleTime = 1000;
                toolTip1.Show("Numbers only", txtInsertNumber, 0, -20, VisibleTime);
            }
        }

        private void txtInsertNumber_TextChanged(object sender, EventArgs e)
        {
            if (lstNumberList.Items.Count < maxNum)
            { this.btnAdd.Enabled = !string.IsNullOrWhiteSpace(this.txtInsertNumber.Text); }
            if (lstNumberList.Items.Count > 0)
            { this.btnSearch.Enabled = !string.IsNullOrWhiteSpace(this.txtInsertNumber.Text); }
        }
        
    }
}

