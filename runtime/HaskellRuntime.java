import java.util.List;
import java.util.Arrays;
import java.util.Locale;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.regex.Pattern;

public class HaskellRuntime {
    private static final Scanner scanner = new Scanner(System.in).useLocale(Locale.US);

    /** ========== ВВОД ========== */

    // Примитивы
    public static int readInt() {
        return scanner.nextInt();
    }

    public static float readFloat() {
        return scanner.nextFloat();
    }

    public static String readString() {
        return scanner.next();
    }

    // Массивы (парсинг строки вида [1,2,3])
    public static int[] readArrayInt() {
        String line = readCleanLine();
        if (line.isEmpty() || line.equals("[]")) return new int[0];
        String[] parts = line.split("[,\\s]+");
        int[] res = new int[parts.length];
        for (int i = 0; i < parts.length; i++) {
            res[i] = Integer.parseInt(parts[i]);
        }
        return res;
    }

    public static float[] readArrayFloat() {
        String line = readCleanLine();
        if (line.isEmpty() || line.equals("[]")) return new float[0];
        String[] parts = line.split("[,\\s]+");
        float[] res = new float[parts.length];
        for (int i = 0; i < parts.length; i++) {
            res[i] = Float.parseFloat(parts[i]);
        }
        return res;
    }
    
    public static String[] readArrayString() {
        String line = readCleanLine();
        if (line.isEmpty() || line.equals("[]")) return new String[0];
        
        // Разделяем по пробелам или запятым
        String[] parts = line.split("[,\\s]+");
        ArrayList<String> cleanStrings = new ArrayList<>();
        
        for (String p : parts) {
            String s = p.trim();
            if (s.isEmpty()) continue;
            // Удаляем кавычки у КАЖДОГО слова, если они есть
            if (s.startsWith("\"") && s.endsWith("\"") && s.length() >= 2) {
                s = s.substring(1, s.length() - 1);
            }
            cleanStrings.add(s);
        }
        return cleanStrings.toArray(new String[0]);
    }

    private static String readCleanLine() {
        // Если перед этим вызывали nextInt(), в буфере мог остаться \n
        if (!scanner.hasNextLine()) return "";
        String line = scanner.nextLine().trim();
        if (line.isEmpty()) line = scanner.nextLine().trim();
        
        if (line.startsWith("[") && line.endsWith("]")) {
            line = line.substring(1, line.length() - 1).trim();
        }
        return line;
    }   


    /** ========== ВЫВОД ========== */ 

    public static void print(String s)     { System.out.println(s); }
    public static void print(int i)        { System.out.println(i); }
    public static void print(float f)      { System.out.println(f); }
    public static void print(Object o)     { System.out.println(o); }
    public static void print(int[] arr)    { System.out.println(Arrays.toString(arr)); }
    public static void print(float[] arr)  { System.out.println(Arrays.toString(arr)); }
    public static void print(String[] arr) { System.out.println(Arrays.toString(arr)); }
}