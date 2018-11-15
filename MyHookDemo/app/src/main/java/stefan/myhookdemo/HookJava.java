package stefan.myhookdemo;

import android.util.Log;


import org.json.JSONArray;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.InputStreamReader;

import de.robv.android.xposed.IXposedHookLoadPackage;
import de.robv.android.xposed.XposedBridge;
import de.robv.android.xposed.callbacks.XC_LoadPackage;

public class HookJava implements IXposedHookLoadPackage {

    @Override
    public void handleLoadPackage(final XC_LoadPackage.LoadPackageParam lpparam) throws Throwable {

        /* 获取配置文件 */
        try
        {
            InputStreamReader reader = new InputStreamReader(new FileInputStream("/data/local/kk/config.json"));
            BufferedReader br = new BufferedReader(reader);

            String line = null;
            StringBuilder builder = new StringBuilder();
            while((line = br.readLine()) != null){
                builder.append(line);
            }
            reader.close();
            br.close();
            JSONObject json = new JSONObject(builder.toString());

            String packageName = null;
            String libName = null;

            JSONArray array = json.getJSONArray("config");
            for (int i = 0; i < array.length(); i++) {
                JSONObject role = array.getJSONObject(i);    //取出数组中的对象

                packageName = role.getString("packageName");
                libName = role.getString("libName");
                if(!packageName.isEmpty() && !libName.isEmpty())
                {
                    if (lpparam.packageName.equals(packageName)) //根据package 加载对应的lib
                    {
                        Log.i("kkk", "packgeName" + packageName + " libName : " + libName);
                        String loadLib = "/data/local/kk/" + libName + ".so";
                        Log.i("kkk", "load lib:" + loadLib);
                        System.load(loadLib);
                    }
                    else
                    {
                        return;
                    }
                }

            }

        }
        catch (Exception e)
        {
            e.printStackTrace();
        }

    }
}
