using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.Threading;

using FableMod.BIG;
using FableMod.ContentManagement;
using FableMod.Gfx.Integration;

namespace FableMod.Content.Forms
{
    delegate void RenderPreviewDelegate();

	public class ControlModel : ControlBIGEntry
	{
		private System.ComponentModel.IContainer components = null;
        private System.Windows.Forms.Button btnSave;
        private GfxModel m_Model;
        private GfxModelLOD m_ModelLOD;
        private TabControl tcModelLOD;
        private TabPage tabPreview;
        private TabPage tabSubMeshes;
        private TabPage tabMaterials;
        private Label lblSubMeshes;
        private ListView lvSubMeshes;
        private Panel pnlSubMeshDisplay;
        private Label lblSubMeshMaterialID;
        private TextBox txtSubMeshMaterialID;
        private Button btnSubMeshApply;
        private Button buttonApply;
        private TextBox txtMaterialID;
        private Label lblMaterialID;
        private Label lblMaterials;
        private ListView lvMaterials;
        private TextBox txtMaterialName;
        private Label lblMaterialName;
        private Panel pnlMaterialDisplay;
        private TextBox txtMaterialBaseTexture;
        private Label lblMaterialBaseTexture;
        private TextBox txtMaterialBumpMap;
        private Label lblMaterialBumpMap;
        private TextBox txtMaterialSpecularMap;
        private Label lblMaterialSpecularMap;
        private TextBox txtMaterialTextureFlags;
        private Label lblMaterialTextureLayers;
        private TextBox txtMaterialGlow;
        private Label lblMaterialGlow;
        private Label lblMaterialUnknown4;
        private Label lblMaterialUnknown3;
        private Label lblMaterialUnknown2;
        private TextBox txtMaterialAlphaTexture;
        private Label lblMaterialUnknown1;
        private TextBox txtMaterialUnknown4;
        private TextBox txtMaterialUnknown3;
        private TextBox txtMaterialUnknown2;
        private Label lblSelectLOD;
        private ComboBox cbSelectLOD;
        private Button btnLoad;
        private Button buttonSelectTexture;
        private Label labelFaces;
        private Label label2;
        private Label labelVertices;
        private Label label1;
        
        private FableMod.Gfx.Integration.GfxController myController;
        private Label label3;
        private TextBox textBoxPhysics;
        private Button buttonBrowse;
        private Button buttonExportImage;
        private FableMod.Gfx.Integration.GfxView myView;
        private Button buttonSelectBumpTexture;
        private CheckBox checkBoxAlpha;
        private bool myBlockEvents = false;

        public ControlModel()
		{
			InitializeComponent();

            myController = new GfxController();
            myView  = new GfxView();
            myController.AddView(myView);
            tabPreview.Controls.Add(myView);
            myView.Dock = DockStyle.Fill;
            myView.BringToFront();
        }
        
        public override void OnActivate()
        {
            base.OnActivate();
        }

        public override void OnDeactivate()
        {
            base.OnDeactivate();
        }

        private void RefreshSubMeshes()
        {
            textBoxPhysics.Text = m_Model.Physics.ToString();

            lvSubMeshes.Clear();

            if (m_ModelLOD != null)
            {
                for (uint i = 0; i < m_ModelLOD.SubMeshCount; i++)
                {
                    SubMesh submesh = m_ModelLOD.get_SubMeshes(i);
                    ListViewItem item = new ListViewItem(i.ToString());
                    item.Tag = submesh;
                    lvSubMeshes.Items.Add(item);
                }
            }
        }
        private void RefreshMaterials()
        {
            lvMaterials.Clear();

            if (m_ModelLOD != null)
            {
                for (uint i = 0; i < m_ModelLOD.MaterialCount; i++)
                {
                    Material mat = m_ModelLOD.get_Materials(i);
                    ListViewItem item = new ListViewItem(mat.Name);
                    item.Tag = mat;
                    lvMaterials.Items.Add(item);
                }
            }

            // Hide the properties.
            pnlMaterialDisplay.Visible = false;
        }

		public override void ApplyChanges()
		{
            try
            {
                m_Model.Physics = UInt32.Parse(textBoxPhysics.Text);
            }
            catch (Exception)
            {
                MessageBox.Show(this, "Invalid physics object.");
                return;
            }

            m_Model.CompileToEntry(m_Entry);

            m_Model = new GfxModel(m_Entry);

            if (cbSelectLOD.SelectedIndex >= 0)
                m_ModelLOD = m_Model.get_LODs(cbSelectLOD.SelectedIndex);
            else
                m_ModelLOD = m_Model.get_LODs(0);

            myController.ResetObjects();
            myController.AddModel(m_ModelLOD);
            myView.FrontCamera();

            myView.Activate(true);

            RefreshSubMeshes();
            RefreshMaterials();
		}
		
        public override AssetEntry BIGEntry
		{
			get
			{
				return base.BIGEntry;
			}
			set
            {
                base.BIGEntry = value;

                myController.ResetObjects();
                cbSelectLOD.Items.Clear();

                if (m_Model != null)
                    m_Model.Dispose();

                m_Model = new GfxModel(m_Entry);
                
                for (int i = 0; i < m_Model.LODCount; i++)
                {
                    cbSelectLOD.Items.Add("LOD "+i.ToString());
                }
                
                cbSelectLOD.SelectedIndex = 0;

                myController.AddModel(m_Model);
                myView.FrontCamera();
                myView.Activate(true);
            }
		}
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{

			//while(m_ContinuousRender.IsAlive)
			//{
			//	Application.DoEvents();
			//}
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
            this.btnSave = new System.Windows.Forms.Button();
            this.tcModelLOD = new System.Windows.Forms.TabControl();
            this.tabPreview = new System.Windows.Forms.TabPage();
            this.tabSubMeshes = new System.Windows.Forms.TabPage();
            this.pnlSubMeshDisplay = new System.Windows.Forms.Panel();
            this.labelFaces = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.labelVertices = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnSubMeshApply = new System.Windows.Forms.Button();
            this.txtSubMeshMaterialID = new System.Windows.Forms.TextBox();
            this.lblSubMeshMaterialID = new System.Windows.Forms.Label();
            this.lblSubMeshes = new System.Windows.Forms.Label();
            this.lvSubMeshes = new System.Windows.Forms.ListView();
            this.tabMaterials = new System.Windows.Forms.TabPage();
            this.pnlMaterialDisplay = new System.Windows.Forms.Panel();
            this.checkBoxAlpha = new System.Windows.Forms.CheckBox();
            this.buttonSelectBumpTexture = new System.Windows.Forms.Button();
            this.buttonSelectTexture = new System.Windows.Forms.Button();
            this.txtMaterialUnknown4 = new System.Windows.Forms.TextBox();
            this.txtMaterialUnknown3 = new System.Windows.Forms.TextBox();
            this.txtMaterialUnknown2 = new System.Windows.Forms.TextBox();
            this.lblMaterialUnknown4 = new System.Windows.Forms.Label();
            this.lblMaterialUnknown3 = new System.Windows.Forms.Label();
            this.lblMaterialUnknown2 = new System.Windows.Forms.Label();
            this.txtMaterialAlphaTexture = new System.Windows.Forms.TextBox();
            this.lblMaterialUnknown1 = new System.Windows.Forms.Label();
            this.buttonApply = new System.Windows.Forms.Button();
            this.txtMaterialGlow = new System.Windows.Forms.TextBox();
            this.lblMaterialGlow = new System.Windows.Forms.Label();
            this.txtMaterialBaseTexture = new System.Windows.Forms.TextBox();
            this.lblMaterialBaseTexture = new System.Windows.Forms.Label();
            this.txtMaterialBumpMap = new System.Windows.Forms.TextBox();
            this.lblMaterialBumpMap = new System.Windows.Forms.Label();
            this.txtMaterialSpecularMap = new System.Windows.Forms.TextBox();
            this.lblMaterialSpecularMap = new System.Windows.Forms.Label();
            this.txtMaterialTextureFlags = new System.Windows.Forms.TextBox();
            this.lblMaterialTextureLayers = new System.Windows.Forms.Label();
            this.txtMaterialName = new System.Windows.Forms.TextBox();
            this.lblMaterialName = new System.Windows.Forms.Label();
            this.txtMaterialID = new System.Windows.Forms.TextBox();
            this.lblMaterialID = new System.Windows.Forms.Label();
            this.lblMaterials = new System.Windows.Forms.Label();
            this.lvMaterials = new System.Windows.Forms.ListView();
            this.lblSelectLOD = new System.Windows.Forms.Label();
            this.cbSelectLOD = new System.Windows.Forms.ComboBox();
            this.btnLoad = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.textBoxPhysics = new System.Windows.Forms.TextBox();
            this.buttonBrowse = new System.Windows.Forms.Button();
            this.buttonExportImage = new System.Windows.Forms.Button();
            this.tcModelLOD.SuspendLayout();
            this.tabSubMeshes.SuspendLayout();
            this.pnlSubMeshDisplay.SuspendLayout();
            this.tabMaterials.SuspendLayout();
            this.pnlMaterialDisplay.SuspendLayout();
            this.SuspendLayout();
            // 
            // lblEntryType
            // 
            this.lblEntryType.Size = new System.Drawing.Size(36, 13);
            this.lblEntryType.Text = "Model";
            // 
            // btnSave
            // 
            this.btnSave.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSave.Location = new System.Drawing.Point(322, 3);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(109, 23);
            this.btnSave.TabIndex = 2;
            this.btnSave.Text = "Export LOD Model";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // tcModelLOD
            // 
            this.tcModelLOD.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tcModelLOD.Controls.Add(this.tabPreview);
            this.tcModelLOD.Controls.Add(this.tabSubMeshes);
            this.tcModelLOD.Controls.Add(this.tabMaterials);
            this.tcModelLOD.Location = new System.Drawing.Point(11, 57);
            this.tcModelLOD.Name = "tcModelLOD";
            this.tcModelLOD.SelectedIndex = 0;
            this.tcModelLOD.Size = new System.Drawing.Size(539, 403);
            this.tcModelLOD.TabIndex = 4;
            this.tcModelLOD.Selected += new System.Windows.Forms.TabControlEventHandler(this.tcModelLOD_Selected);
            // 
            // tabPreview
            // 
            this.tabPreview.Location = new System.Drawing.Point(4, 22);
            this.tabPreview.Name = "tabPreview";
            this.tabPreview.Padding = new System.Windows.Forms.Padding(3);
            this.tabPreview.Size = new System.Drawing.Size(531, 377);
            this.tabPreview.TabIndex = 0;
            this.tabPreview.Text = "Preview";
            this.tabPreview.UseVisualStyleBackColor = true;
            this.tabPreview.Leave += new System.EventHandler(this.tabPreview_Leave);
            this.tabPreview.Enter += new System.EventHandler(this.tabPreview_Enter);
            // 
            // tabSubMeshes
            // 
            this.tabSubMeshes.Controls.Add(this.pnlSubMeshDisplay);
            this.tabSubMeshes.Controls.Add(this.lblSubMeshes);
            this.tabSubMeshes.Controls.Add(this.lvSubMeshes);
            this.tabSubMeshes.Location = new System.Drawing.Point(4, 22);
            this.tabSubMeshes.Name = "tabSubMeshes";
            this.tabSubMeshes.Padding = new System.Windows.Forms.Padding(3);
            this.tabSubMeshes.Size = new System.Drawing.Size(531, 377);
            this.tabSubMeshes.TabIndex = 1;
            this.tabSubMeshes.Text = "SubMeshes";
            this.tabSubMeshes.UseVisualStyleBackColor = true;
            // 
            // pnlSubMeshDisplay
            // 
            this.pnlSubMeshDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlSubMeshDisplay.Controls.Add(this.labelFaces);
            this.pnlSubMeshDisplay.Controls.Add(this.label2);
            this.pnlSubMeshDisplay.Controls.Add(this.labelVertices);
            this.pnlSubMeshDisplay.Controls.Add(this.label1);
            this.pnlSubMeshDisplay.Controls.Add(this.btnSubMeshApply);
            this.pnlSubMeshDisplay.Controls.Add(this.txtSubMeshMaterialID);
            this.pnlSubMeshDisplay.Controls.Add(this.lblSubMeshMaterialID);
            this.pnlSubMeshDisplay.Location = new System.Drawing.Point(189, 24);
            this.pnlSubMeshDisplay.Name = "pnlSubMeshDisplay";
            this.pnlSubMeshDisplay.Size = new System.Drawing.Size(336, 347);
            this.pnlSubMeshDisplay.TabIndex = 2;
            this.pnlSubMeshDisplay.Visible = false;
            // 
            // labelFaces
            // 
            this.labelFaces.AutoSize = true;
            this.labelFaces.Location = new System.Drawing.Point(127, 65);
            this.labelFaces.Name = "labelFaces";
            this.labelFaces.Size = new System.Drawing.Size(10, 13);
            this.labelFaces.TabIndex = 6;
            this.labelFaces.Text = "-";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 65);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(39, 13);
            this.label2.TabIndex = 5;
            this.label2.Text = "Faces:";
            // 
            // labelVertices
            // 
            this.labelVertices.AutoSize = true;
            this.labelVertices.Location = new System.Drawing.Point(127, 41);
            this.labelVertices.Name = "labelVertices";
            this.labelVertices.Size = new System.Drawing.Size(10, 13);
            this.labelVertices.TabIndex = 4;
            this.labelVertices.Text = "-";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 41);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(48, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Vertices:";
            // 
            // btnSubMeshApply
            // 
            this.btnSubMeshApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnSubMeshApply.Location = new System.Drawing.Point(206, 316);
            this.btnSubMeshApply.Name = "btnSubMeshApply";
            this.btnSubMeshApply.Size = new System.Drawing.Size(111, 23);
            this.btnSubMeshApply.TabIndex = 2;
            this.btnSubMeshApply.Text = "Apply Changes";
            this.btnSubMeshApply.UseVisualStyleBackColor = true;
            this.btnSubMeshApply.Click += new System.EventHandler(this.btnSubMeshApply_Click);
            // 
            // txtSubMeshMaterialID
            // 
            this.txtSubMeshMaterialID.Location = new System.Drawing.Point(130, 9);
            this.txtSubMeshMaterialID.Name = "txtSubMeshMaterialID";
            this.txtSubMeshMaterialID.Size = new System.Drawing.Size(168, 20);
            this.txtSubMeshMaterialID.TabIndex = 1;
            // 
            // lblSubMeshMaterialID
            // 
            this.lblSubMeshMaterialID.AutoSize = true;
            this.lblSubMeshMaterialID.Location = new System.Drawing.Point(4, 12);
            this.lblSubMeshMaterialID.Name = "lblSubMeshMaterialID";
            this.lblSubMeshMaterialID.Size = new System.Drawing.Size(61, 13);
            this.lblSubMeshMaterialID.TabIndex = 0;
            this.lblSubMeshMaterialID.Text = "Material ID:";
            // 
            // lblSubMeshes
            // 
            this.lblSubMeshes.AutoSize = true;
            this.lblSubMeshes.Location = new System.Drawing.Point(6, 7);
            this.lblSubMeshes.Name = "lblSubMeshes";
            this.lblSubMeshes.Size = new System.Drawing.Size(66, 13);
            this.lblSubMeshes.TabIndex = 1;
            this.lblSubMeshes.Text = "SubMeshes:";
            // 
            // lvSubMeshes
            // 
            this.lvSubMeshes.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.lvSubMeshes.HideSelection = false;
            this.lvSubMeshes.Location = new System.Drawing.Point(6, 24);
            this.lvSubMeshes.MultiSelect = false;
            this.lvSubMeshes.Name = "lvSubMeshes";
            this.lvSubMeshes.Size = new System.Drawing.Size(177, 347);
            this.lvSubMeshes.TabIndex = 0;
            this.lvSubMeshes.UseCompatibleStateImageBehavior = false;
            this.lvSubMeshes.View = System.Windows.Forms.View.List;
            this.lvSubMeshes.SelectedIndexChanged += new System.EventHandler(this.lvSubMeshes_SelectedIndexChanged);
            // 
            // tabMaterials
            // 
            this.tabMaterials.Controls.Add(this.pnlMaterialDisplay);
            this.tabMaterials.Controls.Add(this.lblMaterials);
            this.tabMaterials.Controls.Add(this.lvMaterials);
            this.tabMaterials.Location = new System.Drawing.Point(4, 22);
            this.tabMaterials.Name = "tabMaterials";
            this.tabMaterials.Padding = new System.Windows.Forms.Padding(3);
            this.tabMaterials.Size = new System.Drawing.Size(531, 377);
            this.tabMaterials.TabIndex = 2;
            this.tabMaterials.Text = "Materials";
            this.tabMaterials.UseVisualStyleBackColor = true;
            // 
            // pnlMaterialDisplay
            // 
            this.pnlMaterialDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pnlMaterialDisplay.Controls.Add(this.checkBoxAlpha);
            this.pnlMaterialDisplay.Controls.Add(this.buttonSelectBumpTexture);
            this.pnlMaterialDisplay.Controls.Add(this.buttonSelectTexture);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialUnknown4);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialUnknown3);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialUnknown2);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialUnknown4);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialUnknown3);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialUnknown2);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialAlphaTexture);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialUnknown1);
            this.pnlMaterialDisplay.Controls.Add(this.buttonApply);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialGlow);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialGlow);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialBaseTexture);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialBaseTexture);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialBumpMap);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialBumpMap);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialSpecularMap);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialSpecularMap);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialTextureFlags);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialTextureLayers);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialName);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialName);
            this.pnlMaterialDisplay.Controls.Add(this.txtMaterialID);
            this.pnlMaterialDisplay.Controls.Add(this.lblMaterialID);
            this.pnlMaterialDisplay.Location = new System.Drawing.Point(189, 24);
            this.pnlMaterialDisplay.Name = "pnlMaterialDisplay";
            this.pnlMaterialDisplay.Size = new System.Drawing.Size(333, 345);
            this.pnlMaterialDisplay.TabIndex = 5;
            this.pnlMaterialDisplay.Visible = false;
            // 
            // checkBoxAlpha
            // 
            this.checkBoxAlpha.AutoSize = true;
            this.checkBoxAlpha.Location = new System.Drawing.Point(130, 183);
            this.checkBoxAlpha.Name = "checkBoxAlpha";
            this.checkBoxAlpha.Size = new System.Drawing.Size(95, 17);
            this.checkBoxAlpha.TabIndex = 27;
            this.checkBoxAlpha.Text = "Alpha Enabled";
            this.checkBoxAlpha.UseVisualStyleBackColor = true;
            // 
            // buttonSelectBumpTexture
            // 
            this.buttonSelectBumpTexture.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSelectBumpTexture.Location = new System.Drawing.Point(290, 81);
            this.buttonSelectBumpTexture.Name = "buttonSelectBumpTexture";
            this.buttonSelectBumpTexture.Size = new System.Drawing.Size(24, 20);
            this.buttonSelectBumpTexture.TabIndex = 26;
            this.buttonSelectBumpTexture.Text = "...";
            this.buttonSelectBumpTexture.UseVisualStyleBackColor = true;
            this.buttonSelectBumpTexture.Click += new System.EventHandler(this.buttonSelectBumpTexture_Click);
            // 
            // buttonSelectTexture
            // 
            this.buttonSelectTexture.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonSelectTexture.Location = new System.Drawing.Point(290, 57);
            this.buttonSelectTexture.Name = "buttonSelectTexture";
            this.buttonSelectTexture.Size = new System.Drawing.Size(24, 20);
            this.buttonSelectTexture.TabIndex = 25;
            this.buttonSelectTexture.Text = "...";
            this.buttonSelectTexture.UseVisualStyleBackColor = true;
            this.buttonSelectTexture.Click += new System.EventHandler(this.buttonSelectTexture_Click);
            // 
            // txtMaterialUnknown4
            // 
            this.txtMaterialUnknown4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialUnknown4.Location = new System.Drawing.Point(130, 283);
            this.txtMaterialUnknown4.Name = "txtMaterialUnknown4";
            this.txtMaterialUnknown4.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialUnknown4.TabIndex = 24;
            // 
            // txtMaterialUnknown3
            // 
            this.txtMaterialUnknown3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialUnknown3.Location = new System.Drawing.Point(130, 257);
            this.txtMaterialUnknown3.Name = "txtMaterialUnknown3";
            this.txtMaterialUnknown3.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialUnknown3.TabIndex = 23;
            // 
            // txtMaterialUnknown2
            // 
            this.txtMaterialUnknown2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialUnknown2.Location = new System.Drawing.Point(130, 231);
            this.txtMaterialUnknown2.Name = "txtMaterialUnknown2";
            this.txtMaterialUnknown2.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialUnknown2.TabIndex = 22;
            // 
            // lblMaterialUnknown4
            // 
            this.lblMaterialUnknown4.AutoSize = true;
            this.lblMaterialUnknown4.Location = new System.Drawing.Point(5, 286);
            this.lblMaterialUnknown4.Name = "lblMaterialUnknown4";
            this.lblMaterialUnknown4.Size = new System.Drawing.Size(62, 13);
            this.lblMaterialUnknown4.TabIndex = 21;
            this.lblMaterialUnknown4.Text = "Unknown4:";
            // 
            // lblMaterialUnknown3
            // 
            this.lblMaterialUnknown3.AutoSize = true;
            this.lblMaterialUnknown3.Location = new System.Drawing.Point(4, 260);
            this.lblMaterialUnknown3.Name = "lblMaterialUnknown3";
            this.lblMaterialUnknown3.Size = new System.Drawing.Size(62, 13);
            this.lblMaterialUnknown3.TabIndex = 20;
            this.lblMaterialUnknown3.Text = "Unknown3:";
            // 
            // lblMaterialUnknown2
            // 
            this.lblMaterialUnknown2.AutoSize = true;
            this.lblMaterialUnknown2.Location = new System.Drawing.Point(4, 234);
            this.lblMaterialUnknown2.Name = "lblMaterialUnknown2";
            this.lblMaterialUnknown2.Size = new System.Drawing.Size(62, 13);
            this.lblMaterialUnknown2.TabIndex = 19;
            this.lblMaterialUnknown2.Text = "Unknown2:";
            // 
            // txtMaterialAlphaTexture
            // 
            this.txtMaterialAlphaTexture.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialAlphaTexture.Location = new System.Drawing.Point(130, 131);
            this.txtMaterialAlphaTexture.Name = "txtMaterialAlphaTexture";
            this.txtMaterialAlphaTexture.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialAlphaTexture.TabIndex = 18;
            this.txtMaterialAlphaTexture.TextChanged += new System.EventHandler(this.txtMaterialAlphaTexture_TextChanged);
            // 
            // lblMaterialUnknown1
            // 
            this.lblMaterialUnknown1.AutoSize = true;
            this.lblMaterialUnknown1.Location = new System.Drawing.Point(3, 134);
            this.lblMaterialUnknown1.Name = "lblMaterialUnknown1";
            this.lblMaterialUnknown1.Size = new System.Drawing.Size(90, 13);
            this.lblMaterialUnknown1.TabIndex = 17;
            this.lblMaterialUnknown1.Text = "Alpha Texture ID:";
            // 
            // buttonApply
            // 
            this.buttonApply.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonApply.Location = new System.Drawing.Point(205, 309);
            this.buttonApply.Name = "buttonApply";
            this.buttonApply.Size = new System.Drawing.Size(109, 23);
            this.buttonApply.TabIndex = 2;
            this.buttonApply.Text = "Apply Changes";
            this.buttonApply.UseVisualStyleBackColor = true;
            this.buttonApply.Click += new System.EventHandler(this.btnMaterialApply_Click);
            // 
            // txtMaterialGlow
            // 
            this.txtMaterialGlow.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialGlow.Location = new System.Drawing.Point(130, 205);
            this.txtMaterialGlow.Name = "txtMaterialGlow";
            this.txtMaterialGlow.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialGlow.TabIndex = 16;
            // 
            // lblMaterialGlow
            // 
            this.lblMaterialGlow.AutoSize = true;
            this.lblMaterialGlow.Location = new System.Drawing.Point(4, 208);
            this.lblMaterialGlow.Name = "lblMaterialGlow";
            this.lblMaterialGlow.Size = new System.Drawing.Size(77, 13);
            this.lblMaterialGlow.TabIndex = 15;
            this.lblMaterialGlow.Text = "Glow Strength:";
            // 
            // txtMaterialBaseTexture
            // 
            this.txtMaterialBaseTexture.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialBaseTexture.Location = new System.Drawing.Point(130, 57);
            this.txtMaterialBaseTexture.Name = "txtMaterialBaseTexture";
            this.txtMaterialBaseTexture.Size = new System.Drawing.Size(154, 20);
            this.txtMaterialBaseTexture.TabIndex = 14;
            this.txtMaterialBaseTexture.TextChanged += new System.EventHandler(this.txtMaterialBaseTexture_TextChanged);
            // 
            // lblMaterialBaseTexture
            // 
            this.lblMaterialBaseTexture.AutoSize = true;
            this.lblMaterialBaseTexture.Location = new System.Drawing.Point(4, 60);
            this.lblMaterialBaseTexture.Name = "lblMaterialBaseTexture";
            this.lblMaterialBaseTexture.Size = new System.Drawing.Size(87, 13);
            this.lblMaterialBaseTexture.TabIndex = 13;
            this.lblMaterialBaseTexture.Text = "Base Texture ID:";
            // 
            // txtMaterialBumpMap
            // 
            this.txtMaterialBumpMap.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialBumpMap.Location = new System.Drawing.Point(130, 81);
            this.txtMaterialBumpMap.Name = "txtMaterialBumpMap";
            this.txtMaterialBumpMap.Size = new System.Drawing.Size(154, 20);
            this.txtMaterialBumpMap.TabIndex = 12;
            this.txtMaterialBumpMap.TextChanged += new System.EventHandler(this.txtMaterialBumpMap_TextChanged);
            // 
            // lblMaterialBumpMap
            // 
            this.lblMaterialBumpMap.AutoSize = true;
            this.lblMaterialBumpMap.Location = new System.Drawing.Point(4, 84);
            this.lblMaterialBumpMap.Name = "lblMaterialBumpMap";
            this.lblMaterialBumpMap.Size = new System.Drawing.Size(75, 13);
            this.lblMaterialBumpMap.TabIndex = 11;
            this.lblMaterialBumpMap.Text = "Bump Map ID:";
            // 
            // txtMaterialSpecularMap
            // 
            this.txtMaterialSpecularMap.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialSpecularMap.Location = new System.Drawing.Point(130, 105);
            this.txtMaterialSpecularMap.Name = "txtMaterialSpecularMap";
            this.txtMaterialSpecularMap.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialSpecularMap.TabIndex = 10;
            this.txtMaterialSpecularMap.TextChanged += new System.EventHandler(this.txtMaterialSpecularMap_TextChanged);
            // 
            // lblMaterialSpecularMap
            // 
            this.lblMaterialSpecularMap.AutoSize = true;
            this.lblMaterialSpecularMap.Location = new System.Drawing.Point(4, 108);
            this.lblMaterialSpecularMap.Name = "lblMaterialSpecularMap";
            this.lblMaterialSpecularMap.Size = new System.Drawing.Size(90, 13);
            this.lblMaterialSpecularMap.TabIndex = 9;
            this.lblMaterialSpecularMap.Text = "Specular Map ID:";
            // 
            // txtMaterialTextureFlags
            // 
            this.txtMaterialTextureFlags.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialTextureFlags.Location = new System.Drawing.Point(130, 157);
            this.txtMaterialTextureFlags.Name = "txtMaterialTextureFlags";
            this.txtMaterialTextureFlags.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialTextureFlags.TabIndex = 8;
            // 
            // lblMaterialTextureLayers
            // 
            this.lblMaterialTextureLayers.AutoSize = true;
            this.lblMaterialTextureLayers.Location = new System.Drawing.Point(5, 160);
            this.lblMaterialTextureLayers.Name = "lblMaterialTextureLayers";
            this.lblMaterialTextureLayers.Size = new System.Drawing.Size(74, 13);
            this.lblMaterialTextureLayers.TabIndex = 7;
            this.lblMaterialTextureLayers.Text = "Texture Flags:";
            // 
            // txtMaterialName
            // 
            this.txtMaterialName.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialName.Location = new System.Drawing.Point(130, 9);
            this.txtMaterialName.Name = "txtMaterialName";
            this.txtMaterialName.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialName.TabIndex = 4;
            // 
            // lblMaterialName
            // 
            this.lblMaterialName.AutoSize = true;
            this.lblMaterialName.Location = new System.Drawing.Point(4, 12);
            this.lblMaterialName.Name = "lblMaterialName";
            this.lblMaterialName.Size = new System.Drawing.Size(78, 13);
            this.lblMaterialName.TabIndex = 3;
            this.lblMaterialName.Text = "Material Name:";
            // 
            // txtMaterialID
            // 
            this.txtMaterialID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtMaterialID.Location = new System.Drawing.Point(130, 33);
            this.txtMaterialID.Name = "txtMaterialID";
            this.txtMaterialID.Size = new System.Drawing.Size(184, 20);
            this.txtMaterialID.TabIndex = 1;
            // 
            // lblMaterialID
            // 
            this.lblMaterialID.AutoSize = true;
            this.lblMaterialID.Location = new System.Drawing.Point(4, 36);
            this.lblMaterialID.Name = "lblMaterialID";
            this.lblMaterialID.Size = new System.Drawing.Size(61, 13);
            this.lblMaterialID.TabIndex = 0;
            this.lblMaterialID.Text = "Material ID:";
            // 
            // lblMaterials
            // 
            this.lblMaterials.AutoSize = true;
            this.lblMaterials.Location = new System.Drawing.Point(6, 7);
            this.lblMaterials.Name = "lblMaterials";
            this.lblMaterials.Size = new System.Drawing.Size(52, 13);
            this.lblMaterials.TabIndex = 4;
            this.lblMaterials.Text = "Materials:";
            // 
            // lvMaterials
            // 
            this.lvMaterials.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.lvMaterials.HideSelection = false;
            this.lvMaterials.Location = new System.Drawing.Point(6, 23);
            this.lvMaterials.MultiSelect = false;
            this.lvMaterials.Name = "lvMaterials";
            this.lvMaterials.Size = new System.Drawing.Size(177, 346);
            this.lvMaterials.TabIndex = 3;
            this.lvMaterials.UseCompatibleStateImageBehavior = false;
            this.lvMaterials.View = System.Windows.Forms.View.List;
            this.lvMaterials.SelectedIndexChanged += new System.EventHandler(this.lvMaterials_SelectedIndexChanged);
            // 
            // lblSelectLOD
            // 
            this.lblSelectLOD.AutoSize = true;
            this.lblSelectLOD.Location = new System.Drawing.Point(137, 8);
            this.lblSelectLOD.Name = "lblSelectLOD";
            this.lblSelectLOD.Size = new System.Drawing.Size(65, 13);
            this.lblSelectLOD.TabIndex = 5;
            this.lblSelectLOD.Text = "Select LOD:";
            // 
            // cbSelectLOD
            // 
            this.cbSelectLOD.FormattingEnabled = true;
            this.cbSelectLOD.Location = new System.Drawing.Point(208, 5);
            this.cbSelectLOD.Name = "cbSelectLOD";
            this.cbSelectLOD.Size = new System.Drawing.Size(80, 21);
            this.cbSelectLOD.TabIndex = 6;
            this.cbSelectLOD.SelectedIndexChanged += new System.EventHandler(this.cbSelectLOD_SelectedIndexChanged);
            // 
            // btnLoad
            // 
            this.btnLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnLoad.Location = new System.Drawing.Point(437, 3);
            this.btnLoad.Name = "btnLoad";
            this.btnLoad.Size = new System.Drawing.Size(109, 23);
            this.btnLoad.TabIndex = 7;
            this.btnLoad.Text = "Import LOD Model";
            this.btnLoad.UseVisualStyleBackColor = true;
            this.btnLoad.Click += new System.EventHandler(this.btnLoad_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(8, 34);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(46, 13);
            this.label3.TabIndex = 8;
            this.label3.Text = "Physics:";
            // 
            // textBoxPhysics
            // 
            this.textBoxPhysics.Location = new System.Drawing.Point(60, 31);
            this.textBoxPhysics.MaxLength = 6;
            this.textBoxPhysics.Name = "textBoxPhysics";
            this.textBoxPhysics.Size = new System.Drawing.Size(62, 20);
            this.textBoxPhysics.TabIndex = 9;
            // 
            // buttonBrowse
            // 
            this.buttonBrowse.Location = new System.Drawing.Point(128, 31);
            this.buttonBrowse.Name = "buttonBrowse";
            this.buttonBrowse.Size = new System.Drawing.Size(25, 20);
            this.buttonBrowse.TabIndex = 10;
            this.buttonBrowse.Text = "...";
            this.buttonBrowse.UseVisualStyleBackColor = true;
            this.buttonBrowse.Click += new System.EventHandler(this.buttonBrowse_Click);
            // 
            // buttonExportImage
            // 
            this.buttonExportImage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonExportImage.Location = new System.Drawing.Point(322, 29);
            this.buttonExportImage.Name = "buttonExportImage";
            this.buttonExportImage.Size = new System.Drawing.Size(224, 23);
            this.buttonExportImage.TabIndex = 11;
            this.buttonExportImage.Text = "Save screenshot as...";
            this.buttonExportImage.UseVisualStyleBackColor = true;
            this.buttonExportImage.Click += new System.EventHandler(this.buttonExportImage_Click);
            // 
            // ControlModel
            // 
            this.Controls.Add(this.buttonExportImage);
            this.Controls.Add(this.buttonBrowse);
            this.Controls.Add(this.textBoxPhysics);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.btnLoad);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.cbSelectLOD);
            this.Controls.Add(this.lblSelectLOD);
            this.Controls.Add(this.tcModelLOD);
            this.EntryType = "Model";
            this.Name = "ControlModel";
            this.Size = new System.Drawing.Size(561, 463);
            this.Controls.SetChildIndex(this.lblEntryType, 0);
            this.Controls.SetChildIndex(this.tcModelLOD, 0);
            this.Controls.SetChildIndex(this.lblSelectLOD, 0);
            this.Controls.SetChildIndex(this.cbSelectLOD, 0);
            this.Controls.SetChildIndex(this.btnSave, 0);
            this.Controls.SetChildIndex(this.btnLoad, 0);
            this.Controls.SetChildIndex(this.label3, 0);
            this.Controls.SetChildIndex(this.textBoxPhysics, 0);
            this.Controls.SetChildIndex(this.buttonBrowse, 0);
            this.Controls.SetChildIndex(this.buttonExportImage, 0);
            this.tcModelLOD.ResumeLayout(false);
            this.tabSubMeshes.ResumeLayout(false);
            this.tabSubMeshes.PerformLayout();
            this.pnlSubMeshDisplay.ResumeLayout(false);
            this.pnlSubMeshDisplay.PerformLayout();
            this.tabMaterials.ResumeLayout(false);
            this.tabMaterials.PerformLayout();
            this.pnlMaterialDisplay.ResumeLayout(false);
            this.pnlMaterialDisplay.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		private void btnSave_Click(object sender, System.EventArgs e)
		{
			if (m_ModelLOD != null)
			{
				SaveFileDialog sfd = new SaveFileDialog();
				sfd.Filter = "DirectX X-File (*.X)|*.X||";
				
				if (sfd.ShowDialog(this) == DialogResult.OK)
				{
                    m_ModelLOD.ExportX(sfd.FileName);
				}
			}
		}

        private void btnLoad_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();

			ofd.Filter = 
                "DirectX X-File (*.x)|*.X|"+
                "Wavefront (*.obj)|*.OBJ||";

            if (ofd.ShowDialog(this) == DialogResult.OK)
            {
                try
                {
                    if (ofd.FilterIndex == 1)
                        m_ModelLOD.ImportX(ofd.FileName);
                    else
                        m_ModelLOD.ImportObj(ofd.FileName);
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }

                m_ModelLOD = m_Model.get_LODs(cbSelectLOD.SelectedIndex);
                
                RefreshSubMeshes();
                RefreshMaterials();
                
                myController.ResetObjects();
                myController.AddModel(m_ModelLOD);
                
                myView.FrontCamera();
            }

            ofd.Dispose();
        }

        private void tcModelLOD_Selected(object sender, TabControlEventArgs e)
        {
            if (e.TabPage == tabPreview)
            {

            }
        }

        private void lvSubMeshes_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvSubMeshes.SelectedItems.Count != 0)
            {
                pnlSubMeshDisplay.Visible = true;
                
                SubMesh mesh = (SubMesh)lvSubMeshes.SelectedItems[0].Tag;

                txtSubMeshMaterialID.Text = mesh.MaterialID.ToString();
                labelVertices.Text = mesh.VertexCount.ToString();
                labelFaces.Text = mesh.FaceCount.ToString();
            }
            else
                pnlSubMeshDisplay.Visible = false;
        }

        private void btnSubMeshApply_Click(object sender, EventArgs e)
        {
            if (lvSubMeshes.SelectedItems.Count == 0)
                return;
            SubMesh submesh = ((SubMesh)lvSubMeshes.SelectedItems[0].Tag);
            try {
                submesh.MaterialID = uint.Parse(txtSubMeshMaterialID.Text);
            } catch(Exception exc)
            {
                MessageBox.Show(this, "Invalid material ID.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void lvMaterials_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (lvMaterials.SelectedItems.Count != 0)
            {
                Material mat = ((Material)lvMaterials.SelectedItems[0].Tag);
                pnlMaterialDisplay.Visible = true;
                txtMaterialID.Text = mat.ID.ToString();
                txtMaterialName.Text = mat.Name;

                myBlockEvents = true;

                txtMaterialBaseTexture.Text = mat.BaseTextureID.ToString();
                txtMaterialBumpMap.Text = mat.BumpMapTextureID.ToString();
                txtMaterialSpecularMap.Text = mat.ReflectionTextureID.ToString();
                txtMaterialTextureFlags.Text = mat.TextureFlags.ToString();
                checkBoxAlpha.Checked = mat.AlphaEnabled != 0;
                txtMaterialGlow.Text = mat.GlowStrength.ToString();

                myBlockEvents = false;

                txtMaterialAlphaTexture.Text = mat.AlphaMapTextureID.ToString();
                txtMaterialUnknown2.Text = mat.Unknown2.ToString();
                txtMaterialUnknown3.Text = mat.Unknown3.ToString();
                txtMaterialUnknown4.Text = mat.Unknown4.ToString();
            }
            else
                pnlMaterialDisplay.Visible = false;
        }

        private void btnMaterialApply_Click(object sender, EventArgs e)
        {
            if (lvMaterials.SelectedItems.Count == 0)
                return;
            
            Material mat = ((Material)lvMaterials.SelectedItems[0].Tag);

            mat.Name = txtMaterialName.Text;

            try
            {
                mat.ID = uint.Parse(txtMaterialID.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(
                    this, 
                    "Invalid material ID.", 
                    "Error", 
                    MessageBoxButtons.OK, 
                    MessageBoxIcon.Error);
            }

            try
            {
                mat.BaseTextureID = 
                    uint.Parse(txtMaterialBaseTexture.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(
                    this, 
                    "Invalid base texture ID.", 
                    "Error", 
                    MessageBoxButtons.OK, 
                    MessageBoxIcon.Error);
            }

            try
            {
                mat.BumpMapTextureID = 
                    uint.Parse(txtMaterialBumpMap.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(
                    this, 
                    "Invalid bump map ID.", 
                    "Error", 
                    MessageBoxButtons.OK, 
                    MessageBoxIcon.Error);
            }

            try
            {
                mat.ReflectionTextureID = 
                    uint.Parse(txtMaterialSpecularMap.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(
                    this, 
                    "Invalid specular map ID.", 
                    "Error", 
                    MessageBoxButtons.OK, 
                    MessageBoxIcon.Error);
            }

            try
            {
                mat.TextureFlags = 
                    uint.Parse(txtMaterialTextureFlags.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, "Invalid max texture layers count.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            try
            {
                mat.AlphaEnabled = (byte)(checkBoxAlpha.Checked ? 1 : 0);
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, "Invalid alpha enabled value.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            try
            {
                mat.GlowStrength = uint.Parse(txtMaterialGlow.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, "Invalid glow strength.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            try
            {
                mat.AlphaMapTextureID = uint.Parse(txtMaterialAlphaTexture.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, "Invalid unknown1 value.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            try
            {
                mat.Unknown2 = byte.Parse(txtMaterialUnknown2.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, "Invalid unknown2 value.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            try
            {
                mat.Unknown3 = byte.Parse(txtMaterialUnknown3.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, "Invalid unknown3 value.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }

            try
            {
                mat.Unknown4 = ushort.Parse(txtMaterialUnknown4.Text);
            }
            catch (Exception exc)
            {
                MessageBox.Show(this, "Invalid unknown4 value.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void cbSelectLOD_SelectedIndexChanged(object sender, EventArgs e)
        {
            m_ModelLOD = m_Model.get_LODs(cbSelectLOD.SelectedIndex);
            RefreshSubMeshes();
            RefreshMaterials();
            myController.ResetObjects();
            myController.AddModel(m_ModelLOD);
        }

        private void SelectTexture(TextBox textBox, uint textureId)
        {
            Material mat = ((Material)lvMaterials.SelectedItems[0].Tag);

            object o = ContentManager.Instance.SelectEntry(
                new ContentManagement.Link(
                    ContentManagement.LinkDestination.MainTextureID,
                    null),
                textureId);

            if (o != null)
            {
                textBox.Text = o.ToString();
            }
        }

        private void buttonSelectTexture_Click(object sender, EventArgs e)
        {
            if (lvMaterials.SelectedItems.Count > 0)
            {
                Material mat = ((Material)lvMaterials.SelectedItems[0].Tag);

                SelectTexture(txtMaterialBaseTexture, mat.BaseTextureID);
            }
        }

        private void buttonBrowse_Click(object sender, EventArgs e)
        {
            object o = ContentManager.Instance.SelectEntry(
                new ContentManagement.Link(
                    ContentManagement.LinkDestination.ModelID,
                    null),
                UInt32.Parse(textBoxPhysics.Text));

            if (o != null)
            {
                textBoxPhysics.Text = o.ToString();
            }
        }

        private void buttonExportImage_Click(object sender, EventArgs e)
        {
            myView.SaveScreenshot();    
        }

        private void tabPreview_Enter(object sender, EventArgs e)
        {
            myView.Activate(true);
        }

        private void tabPreview_Leave(object sender, EventArgs e)
        {
            myView.Activate(false);
        }

        private void SetTextureFlags(TextBox textBox, int flag)
        {
            if (myBlockEvents)
                return;

            int flags = int.Parse(txtMaterialTextureFlags.Text);

            if (textBox.Text == "0")
                flags &= ~flag;
            else
                flags |= flag;
            
            txtMaterialTextureFlags.Text = flags.ToString();
        }

        private void txtMaterialBaseTexture_TextChanged(object sender, EventArgs e)
        {
            SetTextureFlags(txtMaterialBaseTexture, 1);
        }

        private void txtMaterialBumpMap_TextChanged(object sender, EventArgs e)
        {
            SetTextureFlags(txtMaterialBumpMap, 2);
        }

        private void txtMaterialSpecularMap_TextChanged(object sender, EventArgs e)
        {
            SetTextureFlags(txtMaterialSpecularMap, 4);
        }

        private void buttonSelectBumpTexture_Click(object sender, EventArgs e)
        {
            if (lvMaterials.SelectedItems.Count > 0)
            {
                Material mat = ((Material)lvMaterials.SelectedItems[0].Tag);

                SelectTexture(txtMaterialBaseTexture, mat.BumpMapTextureID);
            }
        }

        private void txtMaterialAlphaTexture_TextChanged(object sender, EventArgs e)
        {
            SetTextureFlags(txtMaterialAlphaTexture, 8);

            checkBoxAlpha.Checked = 
                txtMaterialAlphaTexture.Text != "0";
        }
	}
}

