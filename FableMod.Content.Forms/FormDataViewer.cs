using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;

using FableMod.BIN;
using FableMod.ContentManagement;

namespace FableMod.Content.Forms
{
	/// <summary>
	/// Summary description for frmDataView.
	/// </summary>
	public class FormDataViewer : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Panel pnlTop;
		private System.Windows.Forms.Splitter spltTop;
		private System.Windows.Forms.Splitter spltMiddle;
		private System.Windows.Forms.TextBox txtCharData;
		private System.Windows.Forms.TextBox txtHexData;
		private System.Windows.Forms.NumericUpDown nudLineLength;
		private System.Windows.Forms.Label lblLineLength;
        private byte[] m_Data;
        private System.Collections.Generic.SortedList<uint, string> m_CRCList;
        private Splitter spltDataDisplay;
        private Panel pnlDataDisplay;
        private Label lblByte;
        private Label lblUInt;
        private Label lblShort;
        private Label lblUShort;
        private Label lblChar;
        private Label lblInt;
        private Label lblString;
        private Label lblDouble;
        private Label lblFloat;
        private Label lblResults;
        private Button btnRunSearch;
        private TextBox txtSearchID;
        private ListView lvResults;
        private Splitter spltDataResults;
        private Panel pnlDataValueDisplay;
        private Button btnLoadCRCList;

		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.Container components = null;

		public FormDataViewer(byte[] data)
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();
			m_Data = data;
			RefreshDisplay();
			//
			// TODO: Add any constructor code after InitializeComponent call
			//
		}
		public void RefreshDisplay()
		{
            int linecount = 1 + (m_Data.Length / (int)nudLineLength.Value);

            string[] hexlines = new string[linecount];
            string[] chrlines = new string[linecount];

			string hex = "";
			string chr = "";

            linecount = 0;
			for(int i=0;i<m_Data.Length;i++)
			{
				if (m_Data[i] < 0x10)
					hex += "0";
				hex += m_Data[i].ToString("X") + " ";
				if (m_Data[i] < 32)
					chr += ". ";
				else
					chr += new string((char)m_Data[i], 1) + " ";
				if ((i+1)%(int)nudLineLength.Value == 0)
                {
                    hexlines[linecount] = hex;
                    chrlines[linecount] = chr;
                    hex = "";
                    chr = "";
                    linecount++;
					//hex += Environment.NewLine;
					//chr += Environment.NewLine;
				}
            }
            hexlines[linecount] = hex;
            chrlines[linecount] = chr;
			txtHexData.Lines= hexlines;
			txtCharData.Lines = chrlines;
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if(components != null)
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.pnlTop = new System.Windows.Forms.Panel();
            this.lblLineLength = new System.Windows.Forms.Label();
            this.nudLineLength = new System.Windows.Forms.NumericUpDown();
            this.spltTop = new System.Windows.Forms.Splitter();
            this.spltMiddle = new System.Windows.Forms.Splitter();
            this.txtCharData = new System.Windows.Forms.TextBox();
            this.txtHexData = new System.Windows.Forms.TextBox();
            this.spltDataDisplay = new System.Windows.Forms.Splitter();
            this.pnlDataDisplay = new System.Windows.Forms.Panel();
            this.lvResults = new System.Windows.Forms.ListView();
            this.spltDataResults = new System.Windows.Forms.Splitter();
            this.pnlDataValueDisplay = new System.Windows.Forms.Panel();
            this.btnLoadCRCList = new System.Windows.Forms.Button();
            this.txtSearchID = new System.Windows.Forms.TextBox();
            this.lblByte = new System.Windows.Forms.Label();
            this.lblChar = new System.Windows.Forms.Label();
            this.btnRunSearch = new System.Windows.Forms.Button();
            this.lblUShort = new System.Windows.Forms.Label();
            this.lblShort = new System.Windows.Forms.Label();
            this.lblResults = new System.Windows.Forms.Label();
            this.lblUInt = new System.Windows.Forms.Label();
            this.lblString = new System.Windows.Forms.Label();
            this.lblInt = new System.Windows.Forms.Label();
            this.lblDouble = new System.Windows.Forms.Label();
            this.lblFloat = new System.Windows.Forms.Label();
            this.pnlTop.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.nudLineLength)).BeginInit();
            this.pnlDataDisplay.SuspendLayout();
            this.pnlDataValueDisplay.SuspendLayout();
            this.SuspendLayout();
            // 
            // pnlTop
            // 
            this.pnlTop.Controls.Add(this.lblLineLength);
            this.pnlTop.Controls.Add(this.nudLineLength);
            this.pnlTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.pnlTop.Location = new System.Drawing.Point(4, 4);
            this.pnlTop.Name = "pnlTop";
            this.pnlTop.Size = new System.Drawing.Size(702, 56);
            this.pnlTop.TabIndex = 0;
            // 
            // lblLineLength
            // 
            this.lblLineLength.Location = new System.Drawing.Point(8, 32);
            this.lblLineLength.Name = "lblLineLength";
            this.lblLineLength.Size = new System.Drawing.Size(80, 16);
            this.lblLineLength.TabIndex = 1;
            this.lblLineLength.Text = "Byte Per Line:";
            // 
            // nudLineLength
            // 
            this.nudLineLength.Location = new System.Drawing.Point(88, 32);
            this.nudLineLength.Maximum = new decimal(new int[] {
            1024,
            0,
            0,
            0});
            this.nudLineLength.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.nudLineLength.Name = "nudLineLength";
            this.nudLineLength.Size = new System.Drawing.Size(48, 20);
            this.nudLineLength.TabIndex = 0;
            this.nudLineLength.Value = new decimal(new int[] {
            16,
            0,
            0,
            0});
            this.nudLineLength.ValueChanged += new System.EventHandler(this.nudLineLength_ValueChanged);
            // 
            // spltTop
            // 
            this.spltTop.Dock = System.Windows.Forms.DockStyle.Top;
            this.spltTop.Enabled = false;
            this.spltTop.Location = new System.Drawing.Point(4, 60);
            this.spltTop.Name = "spltTop";
            this.spltTop.Size = new System.Drawing.Size(702, 3);
            this.spltTop.TabIndex = 2;
            this.spltTop.TabStop = false;
            // 
            // spltMiddle
            // 
            this.spltMiddle.Dock = System.Windows.Forms.DockStyle.Right;
            this.spltMiddle.Location = new System.Drawing.Point(416, 63);
            this.spltMiddle.Name = "spltMiddle";
            this.spltMiddle.Size = new System.Drawing.Size(3, 240);
            this.spltMiddle.TabIndex = 3;
            this.spltMiddle.TabStop = false;
            // 
            // txtCharData
            // 
            this.txtCharData.Dock = System.Windows.Forms.DockStyle.Right;
            this.txtCharData.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtCharData.HideSelection = false;
            this.txtCharData.Location = new System.Drawing.Point(419, 63);
            this.txtCharData.MaxLength = 4194304;
            this.txtCharData.Multiline = true;
            this.txtCharData.Name = "txtCharData";
            this.txtCharData.ReadOnly = true;
            this.txtCharData.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtCharData.Size = new System.Drawing.Size(287, 240);
            this.txtCharData.TabIndex = 4;
            this.txtCharData.WordWrap = false;
            // 
            // txtHexData
            // 
            this.txtHexData.Dock = System.Windows.Forms.DockStyle.Fill;
            this.txtHexData.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.txtHexData.HideSelection = false;
            this.txtHexData.Location = new System.Drawing.Point(4, 63);
            this.txtHexData.MaxLength = 4194304;
            this.txtHexData.Multiline = true;
            this.txtHexData.Name = "txtHexData";
            this.txtHexData.ReadOnly = true;
            this.txtHexData.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.txtHexData.Size = new System.Drawing.Size(412, 240);
            this.txtHexData.TabIndex = 1;
            this.txtHexData.WordWrap = false;
            this.txtHexData.MouseClick += new System.Windows.Forms.MouseEventHandler(this.txtHexData_MouseClick);
            // 
            // spltDataDisplay
            // 
            this.spltDataDisplay.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.spltDataDisplay.Location = new System.Drawing.Point(4, 303);
            this.spltDataDisplay.Name = "spltDataDisplay";
            this.spltDataDisplay.Size = new System.Drawing.Size(702, 4);
            this.spltDataDisplay.TabIndex = 5;
            this.spltDataDisplay.TabStop = false;
            // 
            // pnlDataDisplay
            // 
            this.pnlDataDisplay.Controls.Add(this.lvResults);
            this.pnlDataDisplay.Controls.Add(this.spltDataResults);
            this.pnlDataDisplay.Controls.Add(this.pnlDataValueDisplay);
            this.pnlDataDisplay.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.pnlDataDisplay.Location = new System.Drawing.Point(4, 307);
            this.pnlDataDisplay.Name = "pnlDataDisplay";
            this.pnlDataDisplay.Size = new System.Drawing.Size(702, 144);
            this.pnlDataDisplay.TabIndex = 6;
            // 
            // lvResults
            // 
            this.lvResults.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvResults.Location = new System.Drawing.Point(239, 0);
            this.lvResults.Name = "lvResults";
            this.lvResults.Size = new System.Drawing.Size(463, 144);
            this.lvResults.TabIndex = 12;
            this.lvResults.UseCompatibleStateImageBehavior = false;
            this.lvResults.View = System.Windows.Forms.View.List;
            this.lvResults.MouseDown += new System.Windows.Forms.MouseEventHandler(this.lvResults_MouseDown);
            // 
            // spltDataResults
            // 
            this.spltDataResults.Location = new System.Drawing.Point(236, 0);
            this.spltDataResults.Name = "spltDataResults";
            this.spltDataResults.Size = new System.Drawing.Size(3, 144);
            this.spltDataResults.TabIndex = 13;
            this.spltDataResults.TabStop = false;
            // 
            // pnlDataValueDisplay
            // 
            this.pnlDataValueDisplay.Controls.Add(this.btnLoadCRCList);
            this.pnlDataValueDisplay.Controls.Add(this.txtSearchID);
            this.pnlDataValueDisplay.Controls.Add(this.lblByte);
            this.pnlDataValueDisplay.Controls.Add(this.lblChar);
            this.pnlDataValueDisplay.Controls.Add(this.btnRunSearch);
            this.pnlDataValueDisplay.Controls.Add(this.lblUShort);
            this.pnlDataValueDisplay.Controls.Add(this.lblShort);
            this.pnlDataValueDisplay.Controls.Add(this.lblResults);
            this.pnlDataValueDisplay.Controls.Add(this.lblUInt);
            this.pnlDataValueDisplay.Controls.Add(this.lblString);
            this.pnlDataValueDisplay.Controls.Add(this.lblInt);
            this.pnlDataValueDisplay.Controls.Add(this.lblDouble);
            this.pnlDataValueDisplay.Controls.Add(this.lblFloat);
            this.pnlDataValueDisplay.Dock = System.Windows.Forms.DockStyle.Left;
            this.pnlDataValueDisplay.Location = new System.Drawing.Point(0, 0);
            this.pnlDataValueDisplay.Name = "pnlDataValueDisplay";
            this.pnlDataValueDisplay.Size = new System.Drawing.Size(236, 144);
            this.pnlDataValueDisplay.TabIndex = 14;
            // 
            // btnLoadCRCList
            // 
            this.btnLoadCRCList.Location = new System.Drawing.Point(130, 112);
            this.btnLoadCRCList.Name = "btnLoadCRCList";
            this.btnLoadCRCList.Size = new System.Drawing.Size(100, 23);
            this.btnLoadCRCList.TabIndex = 12;
            this.btnLoadCRCList.Text = "Load CRC List";
            this.btnLoadCRCList.UseVisualStyleBackColor = true;
            this.btnLoadCRCList.Click += new System.EventHandler(this.btnLoadCRCList_Click);
            // 
            // txtSearchID
            // 
            this.txtSearchID.Location = new System.Drawing.Point(130, 6);
            this.txtSearchID.Name = "txtSearchID";
            this.txtSearchID.Size = new System.Drawing.Size(100, 20);
            this.txtSearchID.TabIndex = 10;
            // 
            // lblByte
            // 
            this.lblByte.AutoSize = true;
            this.lblByte.Location = new System.Drawing.Point(5, 7);
            this.lblByte.Name = "lblByte";
            this.lblByte.Size = new System.Drawing.Size(34, 13);
            this.lblByte.TabIndex = 0;
            this.lblByte.Text = "Byte: ";
            // 
            // lblChar
            // 
            this.lblChar.AutoSize = true;
            this.lblChar.Location = new System.Drawing.Point(5, 22);
            this.lblChar.Name = "lblChar";
            this.lblChar.Size = new System.Drawing.Size(35, 13);
            this.lblChar.TabIndex = 1;
            this.lblChar.Text = "Char: ";
            // 
            // btnRunSearch
            // 
            this.btnRunSearch.Location = new System.Drawing.Point(130, 27);
            this.btnRunSearch.Name = "btnRunSearch";
            this.btnRunSearch.Size = new System.Drawing.Size(100, 23);
            this.btnRunSearch.TabIndex = 11;
            this.btnRunSearch.Text = "Run Ref Search";
            this.btnRunSearch.UseVisualStyleBackColor = true;
            this.btnRunSearch.Click += new System.EventHandler(this.btnRunSearch_Click);
            // 
            // lblUShort
            // 
            this.lblUShort.AutoSize = true;
            this.lblUShort.Location = new System.Drawing.Point(5, 37);
            this.lblUShort.Name = "lblUShort";
            this.lblUShort.Size = new System.Drawing.Size(46, 13);
            this.lblUShort.TabIndex = 2;
            this.lblUShort.Text = "UShort: ";
            // 
            // lblShort
            // 
            this.lblShort.AutoSize = true;
            this.lblShort.Location = new System.Drawing.Point(5, 52);
            this.lblShort.Name = "lblShort";
            this.lblShort.Size = new System.Drawing.Size(38, 13);
            this.lblShort.TabIndex = 3;
            this.lblShort.Text = "Short: ";
            // 
            // lblResults
            // 
            this.lblResults.AutoSize = true;
            this.lblResults.Location = new System.Drawing.Point(185, 67);
            this.lblResults.Name = "lblResults";
            this.lblResults.Size = new System.Drawing.Size(45, 13);
            this.lblResults.TabIndex = 9;
            this.lblResults.Text = "Results:";
            // 
            // lblUInt
            // 
            this.lblUInt.AutoSize = true;
            this.lblUInt.Location = new System.Drawing.Point(5, 67);
            this.lblUInt.Name = "lblUInt";
            this.lblUInt.Size = new System.Drawing.Size(33, 13);
            this.lblUInt.TabIndex = 4;
            this.lblUInt.Text = "UInt: ";
            // 
            // lblString
            // 
            this.lblString.AutoEllipsis = true;
            this.lblString.AutoSize = true;
            this.lblString.Location = new System.Drawing.Point(5, 127);
            this.lblString.Name = "lblString";
            this.lblString.Size = new System.Drawing.Size(37, 13);
            this.lblString.TabIndex = 8;
            this.lblString.Text = "String:";
            // 
            // lblInt
            // 
            this.lblInt.AutoSize = true;
            this.lblInt.Location = new System.Drawing.Point(5, 82);
            this.lblInt.Name = "lblInt";
            this.lblInt.Size = new System.Drawing.Size(25, 13);
            this.lblInt.TabIndex = 5;
            this.lblInt.Text = "Int: ";
            // 
            // lblDouble
            // 
            this.lblDouble.AutoSize = true;
            this.lblDouble.Location = new System.Drawing.Point(5, 112);
            this.lblDouble.Name = "lblDouble";
            this.lblDouble.Size = new System.Drawing.Size(44, 13);
            this.lblDouble.TabIndex = 7;
            this.lblDouble.Text = "Double:";
            // 
            // lblFloat
            // 
            this.lblFloat.AutoSize = true;
            this.lblFloat.Location = new System.Drawing.Point(5, 97);
            this.lblFloat.Name = "lblFloat";
            this.lblFloat.Size = new System.Drawing.Size(33, 13);
            this.lblFloat.TabIndex = 6;
            this.lblFloat.Text = "Float:";
            // 
            // FormDataViewer
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
            this.ClientSize = new System.Drawing.Size(710, 455);
            this.Controls.Add(this.txtHexData);
            this.Controls.Add(this.spltMiddle);
            this.Controls.Add(this.txtCharData);
            this.Controls.Add(this.spltTop);
            this.Controls.Add(this.pnlTop);
            this.Controls.Add(this.spltDataDisplay);
            this.Controls.Add(this.pnlDataDisplay);
            this.Name = "FormDataViewer";
            this.Padding = new System.Windows.Forms.Padding(4);
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Data Viewer";
            this.Load += new System.EventHandler(this.frmDataView_Load);
            this.pnlTop.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.nudLineLength)).EndInit();
            this.pnlDataDisplay.ResumeLayout(false);
            this.pnlDataValueDisplay.ResumeLayout(false);
            this.pnlDataValueDisplay.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void nudLineLength_ValueChanged(object sender, System.EventArgs e)
		{
			RefreshDisplay();
		}

        private void frmDataView_Load(object sender, EventArgs e)
        {

        }

        private void txtHexData_MouseClick(object sender, MouseEventArgs e)
        {
            Member b= new Member(MemberType.BYTE, "byte", null, null);
            Member c= new Member(MemberType.CHAR, "char", null, null);
            Member us= new Member(MemberType.USHORT, "ushort", null, null);
            Member s= new Member(MemberType.SHORT, "short", null, null);
            Member ui= new Member(MemberType.UINT, "uint", null, null);
            Member i= new Member(MemberType.INT, "int", null, null);
            Member f= new Member(MemberType.FLOAT, "float", null, null);
            Member d= new Member(MemberType.DOUBLE, "double", null, null);
            Member str= new Member(MemberType.STRING, "string", null, null);

            int offset = txtHexData.SelectionStart/(((int)nudLineLength.Value*3)+Environment.NewLine.Length);
            offset = ((int)nudLineLength.Value) * offset + (txtHexData.SelectionStart - offset*(((int)nudLineLength.Value*3)+Environment.NewLine.Length)) / 3;
            
            try
            {
                b.ReadIn(m_Data, offset);
                lblByte.Text = "Byte: " + b.Value.ToString();
            }
            catch (Exception)
            {
                lblByte.Text = "Byte: Error";
            }
            try
            {
                c.ReadIn(m_Data, offset);
                lblChar.Text = "Char: " + c.Value.ToString();
            }
            catch (Exception)
            {
                lblChar.Text = "Char: Error";
            }

            try
            {
                us.ReadIn(m_Data, offset);
                lblUShort.Text = "UShort: " + us.Value.ToString();
            }
            catch (Exception)
            {
                lblUShort.Text = "UShort: Error";
            }
            try
            {
                s.ReadIn(m_Data, offset);
                lblShort.Text = "Short: " + s.Value.ToString();
            }
            catch (Exception)
            {
                lblShort.Text = "Short: Error";
            }
            try
            {
                ui.ReadIn(m_Data, offset);
                lblUInt.Text = "UInt: " + ui.Value.ToString();
                txtSearchID.Text = ui.Value.ToString();
            }
            catch (Exception)
            {
                lblUInt.Text = "UInt: Error";
            }
            try
            {
                i.ReadIn(m_Data, offset);
                lblInt.Text = "Int: " + i.Value.ToString();
            }
            catch (Exception)
            {
                lblInt.Text = "Int: Error";
            }
            try
            {
                f.ReadIn(m_Data, offset);
                lblFloat.Text = "Float: " + f.Value.ToString();
            }
            catch (Exception)
            {
                lblFloat.Text = "Float: Error";
            }
            try
            {
                d.ReadIn(m_Data, offset);
                lblDouble.Text = "Double: " + d.Value.ToString();
            }
            catch (Exception)
            {
                lblDouble.Text = "Double: Error";
            }
            try
            {
                str.ReadIn(m_Data, offset);
                lblString.Text = "String: " + str.Value.ToString();
            }
            catch (Exception)
            {
                lblString.Text = "String: Error";
            }
        }

        private void btnRunSearch_Click(object sender, EventArgs e)
        {
            UInt32 id = 0;

            try
            {
                id = UInt32.Parse(txtSearchID.Text);
            }
            catch(Exception)
            {
                MessageBox.Show(this, 
                    "Error parsing search id.", "Invalid ID");
                lvResults.Clear();
                return;
            }
            
            lvResults.Clear();

            ContentObject o = null; //ContentManager.Instance.FindEntry(id);

            if (o != null)
            {
                ListViewItem item = new ListViewItem(o.Name);
                item.Tag = o.Object;
                lvResults.Items.Add(item);
            }
            
            if (m_CRCList != null)
            {
                string val;

                if (m_CRCList.TryGetValue(id, out val))
                {
                    ListViewItem item = 
                        new ListViewItem("CRC Match: \"" + val + "\"");
                    
                    lvResults.Items.Add(item);
                }
            }
        }

        private void lvResults_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                if (e.Clicks == 2)
                {
                    ListViewItem item = lvResults.GetItemAt(e.X,e.Y);
                    if (item != null && item.Tag != null)
                    {
                        ContentManager.Instance.ShowEntry(item.Tag, false);
                    }
                }
            }
        }

        private void btnLoadCRCList_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            
            ofd.Filter = "*.txt|*.txt";
            ofd.InitialDirectory = AppDomain.CurrentDomain.BaseDirectory;

            if (ofd.ShowDialog() == DialogResult.OK)
            {
                m_CRCList = new System.Collections.Generic.SortedList<uint, string>();
                System.IO.StreamReader sr = System.IO.File.OpenText(ofd.FileName);
                
                string line;
                
                while ((line = sr.ReadLine()) != null)
                {
                    string[] vals = line.Split("	".ToCharArray());
                    m_CRCList.Add(uint.Parse(vals[0]), vals[1]);
                }
                
                sr.Close();
            }
        }
    }
}
