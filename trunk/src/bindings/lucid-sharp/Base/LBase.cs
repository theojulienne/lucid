using System;

namespace Lucid.Base
{
	//FIXME- unref/ref bullshit	
	public abstract class LBase: IWrapper, IDisposable
	{
		IntPtr m_handle;
		private bool m_disposed = false;

		public LBase(IntPtr handle)
		{
			if(handle == IntPtr.Zero)
				throw new ArgumentException("handle");
			m_handle = handle;
		}

		public IntPtr Handle
		{
			get
			{
				return m_handle;
			}
		}
		
		~LBase()
		{
			Dispose(false);
		}

		public void Dispose()
		{
			Dispose(true);
			GC.SuppressFinalize(this);
		}

		private void Dispose(bool disposeManagedResources)
		{
			if(!m_disposed)
			{
				if(disposeManagedResources)
					CleanupManaged();
				CleanupInternal();
				m_disposed = true;				
			}
		}

		protected virtual void CleanupManaged()
		{
			Console.WriteLine("LBase.CleanupManaged()");
		}

		protected virtual void CleanupInternal()
		{
			Console.WriteLine("LBase.CleanupInternal()");
		}
	}
}

