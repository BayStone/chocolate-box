using System;
using System.Windows.Forms;

namespace FableMod.Forms
{
	/// <summary>
	/// Messages is a singleton class that provides an easy to use
    /// interface for message boxes.
	/// </summary>
	public class Messages
	{
		public static Form myForm = null;

        /// <summary>
        ///  Set application name.
        /// </summary>
        /// <param name="app">Name of the application.</param>
		public static void SetForm(Form form)
		{
			myForm = form;
		}
        
        public static void Info(string message)
		{
            MessageBox.Show(
                null, 
                message, 
                myForm.Text,
				MessageBoxButtons.OK,
                MessageBoxIcon.Information);
		}
		
		public static void Error(string message)
		{
            MessageBox.Show(
                null, 
                message,
                myForm.Text,
				MessageBoxButtons.OK,
                MessageBoxIcon.Error);
		}

        public static void Error(Exception ex)
        {
            MessageBox.Show(
                null,
                ex.Message,
                myForm.Text,
                MessageBoxButtons.OK,
                MessageBoxIcon.Error);
        }

		public static void Warning(string message)
		{
            MessageBox.Show(
                null, 
                message, 
                myForm.Text,
				MessageBoxButtons.OK,
                MessageBoxIcon.Warning);
		}
	}
}
