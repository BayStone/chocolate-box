using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;
using System.Collections;

using FableMod.ContentManagement;

namespace ChocolateBox
{
    public partial class FormTreeFileController : FormFileController
    {
        static bool myStaticFullNames = Settings.GetBool(
            "TreeView", "FullNames", false);

        protected class NodeSorter : IComparer
        {
            // Compare the length of the strings, or the strings
            // themselves, if they are the same length.
            public int Compare(object x, object y)
            {
                TreeNode tx = x as TreeNode;
                TreeNode ty = y as TreeNode;

                return string.Compare(ty.Name, tx.Name);
            }
        }

        delegate void AddNodeDelegate(TreeNode parent, TreeNode node);

        private bool myUnderscoreSeparator = true;
        private int myMinTextLength = 3;
        private int myCurrentFind = -1;
        protected List<TreeNode> myFindNodes = new List<TreeNode>();
        
        public FormTreeFileController()
        {
            InitializeComponent();

            ImageList list = new ImageList();

            try
            {
                string iconsDir = Settings.DataDirectory+"icons\\";

                list.Images.Add(Image.FromFile(iconsDir+"object.bmp"));
                list.Images.Add(Image.FromFile(iconsDir+"objectpart.bmp"));
                list.ImageSize = new Size(16, 16);
                list.TransparentColor = Color.Magenta;
            }
            catch (Exception)
            {
            }
            
            treeView.ImageList = list;
        }

        [Browsable(true)]
        [Category("Appearance")]
        [Description("Separate tree nodes with underscores")]
        public bool UnderscoreSeparator
        {
            get { return myUnderscoreSeparator; }
            set { myUnderscoreSeparator = value; }
        }

        [Browsable(true)]
        [Category("Appearance")]
        [Description("Minimum length of a tree node text")]
        public int MinTextLength
        {
            get { return myMinTextLength; }
            set { myMinTextLength = value; }
        }


        protected virtual void ShowObject(object o)
        {
        }

        protected virtual bool DeleteObject(object o)
        {
            return false;
        }
        
        protected void SelectNodesByID(UInt32 id, List<TreeNode> nodes)
        {
            for (int i = 0; i < treeView.Nodes.Count; ++i)
            {
                if (SelectNodesByID(treeView.Nodes[i], id, nodes))
                    break;
            }
        }

        protected bool SelectNodesByID(
            TreeNode node, 
            UInt32 id, 
            List<TreeNode> nodes)
        {
            if (node.Tag != null)
            {
                if (id == GetObjectID(node.Tag))
                {
                    nodes.Add(node);
                    return true;
                }
            }

            for (int i = 0; i < node.Nodes.Count; ++i)
            {
                if (SelectNodesByID(node.Nodes[i], id, nodes))
                    return true;
            }

            return false;
        }

        protected void LocateNodes(
            TreeNode root,
            Regex regex,
            List<TreeNode> nodes)
        {
            if (regex.IsMatch(root.Name))
                nodes.Add(root);

            for (int i = 0; i < root.Nodes.Count; ++i)
            {
                LocateNodes(root.Nodes[i], regex, nodes);
            }
        }

        protected void AddNode(TreeNode parent, TreeNode node)
        {
            if (InvokeRequired)
            {
                AddNodeDelegate cb = new AddNodeDelegate(AddNode);
                Invoke(cb, parent, node);
            }
            else
            {
                if (parent == null)
                    treeView.Nodes.Add(node);
                else
                    parent.Nodes.Add(node);
            }
        }


        protected TreeNode AddToTreeRec(
            TreeNode root, string name, object o)
        {
            // Check for those nasty dev files.
            // It's hard to find a place for this.

            bool dev = false;

            int index = name.IndexOf("[\\");

            if (index == 0)
            {
                name = name.Substring(2);
                dev = true;
            }

            return AddToTreeRec(root, name, name, o, dev);
        }
        
        private TreeNode AddEntryToTree(
            TreeNode parent, string fullName, string name, object o)
        {
            Int32 id = GetObjectID(o);

            StringBuilder sb = new StringBuilder(name);

            if (id > 0)
                sb.AppendFormat(" [{0}]", id);

            TreeNode newNode = new TreeNode();

            newNode.Name = fullName;
            newNode.Text = sb.ToString();
            newNode.Tag  = o;
            newNode.ImageIndex = 0;
            newNode.SelectedImageIndex = 0;

            sb = null;

            AddNode(parent, newNode);

            return newNode;
        }
        
        protected TreeNode AddToTree(TreeNode parent, string name, object o)
        {
            if (myStaticFullNames)
                return AddEntryToTree(parent, name, name, o);
            else    
                return AddToTreeRec(parent, name, o);
        }

        private TreeNode AddToTreeRec(
            TreeNode root, string fullName, string name, object o, bool dev)
        {
            if (name.Length == 0)
                name = "<UNNAMED>";

            int index = 0;
            
            if (dev)
                index = name.IndexOf("\\");
            else if (UnderscoreSeparator)
                index = name.IndexOf("_");

            if (index > 0 &&
                (myMinTextLength == 0 || index >= myMinTextLength) &&
                index < name.Length-myMinTextLength)
            {
                string subName = name.Substring(0, index);
                
                for (int i = 0; i < root.Nodes.Count; ++i)
                {
                    if (root.Nodes[i].Text == subName &&
                        root.Nodes[i].Tag == null)
                    {
                        return AddToTreeRec(
                            root.Nodes[i],
                            fullName,
                            name.Substring(index+1),
                            o,
                            dev);
                    }
                }

                TreeNode node = new TreeNode();

                node.Name = subName;
                node.Text = subName;
                node.Tag  = null;
                node.ImageIndex = 1;
                node.SelectedImageIndex = 1;

                AddNode(root, node);

                return AddToTreeRec(
                    node, fullName, name.Substring(index+1), o, dev);
            }
       
            return AddEntryToTree(root, fullName, name, o);
        }
        
        protected virtual Int32 GetObjectID(object o)
        {
            return -1;
        }

        protected virtual void FindNodes(string pattern, List<TreeNode> nodes)
        {
            Regex regex = new Regex(pattern, RegexOptions.IgnoreCase);

            for (int i = 0; i < treeView.Nodes.Count; ++i)
                LocateNodes(treeView.Nodes[i], regex, nodes);

            regex = null;
        }

        protected virtual bool ObjectIsModified(object o)
        {
            return false;
        }

        protected void ClearFound()
        {
            // Restore old found nodes.

            for (int i = 0; i < myFindNodes.Count; ++i)
            {
                myFindNodes[i].BackColor = treeView.BackColor;
            }

            myFindNodes.Clear();
        }

        protected void SetupFound()
        {
            if (myFindNodes.Count > 0)
            {
                for (int i = 0; i < myFindNodes.Count; ++i)
                    myFindNodes[i].BackColor = Color.LightGray;
                
                myCurrentFind = 0;

                treeView.SelectedNode = myFindNodes[0];
                treeView.SelectedNode.EnsureVisible();
            }
        }

        protected void FindByName()
        {
            FormFind form = new FormFind();

            if (form.ShowDialog() == DialogResult.OK)
            {
                ClearFound();

                FindNodes(form.textBoxName.Text, myFindNodes);

                SetupFound();

                treeView.Focus();
            }

            form.Dispose();
        }

        protected void FindByID()
        {
            FormFind form = new FormFind();

            form.labelInfo.Text = "ID:";

            if (form.ShowDialog() == DialogResult.OK)
            {
                ClearFound();

                SelectNodesByID(
                    UInt32.Parse(form.textBoxName.Text), myFindNodes);

                SetupFound();

                treeView.Focus();
            }

            form.Dispose();
        }

        private void LocateModified(TreeNode node)
        {
            if (node.Tag != null)
            {
                if (ObjectIsModified(node.Tag))
                    myFindNodes.Add(node);
            }

            for (int i = 0; i < node.Nodes.Count; ++i)
            {
                LocateModified(node.Nodes[i]);
            }
        }

        protected void FindModified()
        {
            ClearFound();

            for (int i = 0; i < treeView.Nodes.Count; ++i)
            {
                LocateModified(treeView.Nodes[i]);
            }
            
            if (myFindNodes.Count > 0)
            {
                FormMain.Instance.InfoMessage(string.Format(
                    "{0} object(s) found.", myFindNodes.Count));
                
                SetupFound();
            }
            else
                FormMain.Instance.InfoMessage("No objects found.");
        }

        private void treeView_DoubleClick(object sender, EventArgs e)
        {
            TreeNode node = treeView.SelectedNode;
                
            if (node != null && node.Tag != null)
                ShowObject(node.Tag);
        }

        private void treeView_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.F3)
            {
                if (myFindNodes.Count == 0)
                {
                    // Start the search.
                    FindByName();
                }
                else
                    FindNext();
                
                e.Handled = true;
            }
            else if (e.KeyCode == Keys.Delete)
            {
                DeleteSelected();
                e.Handled = true;
            }
            else
                e.Handled = false;
        }

        protected void FindNext()
        {
            if (myFindNodes.Count > 0)
            {
                ++myCurrentFind;

                if (myCurrentFind == myFindNodes.Count)
                    myCurrentFind = 0;

                treeView.SelectedNode = myFindNodes[myCurrentFind];
            }
        }

        protected void DeleteSelected()
        {
            TreeNode node = treeView.SelectedNode;

            if (node != null && node.Tag != null)
            {
                if (DeleteObject(node.Tag))
                {
                    TreeNode next = node.NextNode;
                    treeView.SelectedNode = node.NextNode;
                    node.Remove();
                    node = null;
                }
            }
        }

        protected ContentObject CreateDragObject(TreeNode node)
        {
            return FileDatabase.Instance.GetContentObject(node.Tag);
        }

        private void treeView_ItemDrag(object sender, ItemDragEventArgs e)
        {
            if (treeView.SelectedNode != null &&
                treeView.SelectedNode.Tag != null)
            {
                ContentObject o = CreateDragObject(treeView.SelectedNode);

                if (o != null)
                {
                    DoDragDrop(o, DragDropEffects.Copy);
                }
            }
        }
    }
}