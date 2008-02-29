using System;
using System.IO;
using System.Reflection;
using System.Collections.Generic;

namespace Lucid
{
    internal static class Native
    {
        public static INativeFactory Factory = null;
    
        public static void Load()
        {
            if(Factory != null)
                return;
        
            List<string> directories = new List<string>();
            Assembly asm = Assembly.GetExecutingAssembly();
            
            string this_asm_path = asm.Location;
            directories.Add(Path.GetDirectoryName(this_asm_path));

            NativeFactoryAttribute impl_attr = null;

            /*
            if(Assembly.GetExecutingAssembly().GlobalAssemblyCache) 
            {
                string [] path_parts = directories[0].Split(Path.DirectorySeparatorChar);
                string new_path = Path.DirectorySeparatorChar.ToString();
                string root = Path.GetPathRoot (this_asm_path);
                
                if (root.StartsWith (path_parts[0])) 
                    path_parts[0] = root;

                for(int i = 0; i < path_parts.Length - 4; i++) 
                    new_path = Path.Combine(new_path, path_parts[i]);
                
                directories.Add(Path.Combine(new_path, "mono-zeroconf"));
            }
            */

            foreach(string directory in directories) 
            {
                foreach(string file in Directory.GetFiles(directory, "Lucid.*.dll")) 
                {
                    if(Path.GetFileName(file) != Path.GetFileName(this_asm_path)) 
                    {
                        Assembly provider_asm = null;
                        try
                        {
                            provider_asm = Assembly.LoadFile(file);
                        }
                        catch(Exception)
                        {
                            continue;
                        }                        
                        foreach(Attribute attr in provider_asm.GetCustomAttributes(false)) 
                        {
                            impl_attr = (attr as NativeFactoryAttribute);
                            if(impl_attr != null) 
                                goto done;    
                        }
                    }
                }
            }
done:
            if(impl_attr == null || impl_attr.Type == null) 
                throw new InvalidOperationException("Couldn't find native library.");
    
            Type t = impl_attr.Type;
            MethodInfo method = t.GetMethod("Get", BindingFlags.Static | BindingFlags.Public);
            
            Factory = (INativeFactory) method.Invoke(null, null);
        }
    }
}

