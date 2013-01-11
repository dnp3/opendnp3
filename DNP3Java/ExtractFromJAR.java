package org.psi.dnp3;

import java.io.*;
import java.util.jar.*;
import java.util.zip.*;
import java.io.File;
import java.io.UnsupportedEncodingException;
import java.net.URLDecoder;
import java.nio.charset.Charset;

class ExtractFromJAR {

  public String extractToLocal(String file) throws IOException, FileNotFoundException, IllegalStateException{
    String home = findPathJar(ExtractFromJAR.class);
    System.out.println("Home = "+ home);
    JarFile jar = new JarFile(home);
    ZipEntry entry = jar.getEntry(file);
    File jarFile = new File(home);
    File efile = new File(jarFile.getParent(), entry.getName());

    InputStream in = new BufferedInputStream(jar.getInputStream(entry));
    OutputStream out = new BufferedOutputStream(new FileOutputStream(efile));
    byte[] buffer = new byte[2048];
    for (;;)  {
      int nBytes = in.read(buffer);
      if (nBytes <= 0) break;
      out.write(buffer, 0, nBytes);
    }
    out.flush();
    out.close();
    in.close();
    return efile.getAbsolutePath();
     

  }

  public static String findPathJar(Class<?> context) throws IllegalStateException {
    return new File(context.getProtectionDomain().getCodeSource().getLocation().getPath()).getAbsolutePath();
  }
}
