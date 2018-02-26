using System;
using System.Collections.Generic;
using System.Text;

namespace ChocolateBox
{
    /// <summary>
    /// An abstract base class for "processing".
    /// A task that takes a little bit of time.
    /// </summary>
    public class Processor
    {
        public Processor()
        {
        }

        public virtual void Run(Progress progress)
        {
        }
    }
}
