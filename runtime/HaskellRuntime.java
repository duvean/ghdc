import java.util.List;
import java.util.Arrays;
import java.util.Locale;
import java.util.Scanner;
import java.util.ArrayList;
import java.util.regex.Pattern;
import java.lang.reflect.Array;
import java.lang.reflect.Method;

public class HaskellRuntime {
    private static final Scanner scanner = new Scanner(System.in).useLocale(Locale.US);



    /* ========== ВВОД ========== */

    // Примитивы
    public static int readInt() { return scanner.nextInt(); }
    public static float readFloat() { return scanner.nextFloat(); }
    public static String readString() { return scanner.next(); }

    // Массивы
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



    /* ========== ВЫВОД ========== */ 

    public static void print(int i)         { System.out.println(i); }
    public static void print(float f)       { System.out.println(f); }
    public static void print(String s)      { System.out.println(s); }
    public static void print(boolean b)     { System.out.println(b); }
    public static void print(int[] arr)     { System.out.println(Arrays.toString(arr)); }
    public static void print(float[] arr)   { System.out.println(Arrays.toString(arr)); }
    public static void print(String[] arr)  { System.out.println(Arrays.toString(arr)); }
    public static void print(Object[] arr)  { System.out.println(Arrays.deepToString(arr)); }
    public static void print(boolean[] arr) { System.out.println(Arrays.toString(arr)); }
    public static void print(Object obj) {
        if (obj instanceof Integer) {
            System.out.println(((Integer)obj).intValue());
        } else if (obj instanceof Float) {
            System.out.println(((Float)obj).floatValue());
        } else {
            System.out.println(obj);
        }
    }



    /* ========== ФУНКЦИИ ДЛЯ РАБОТЫ С МАССИВАМИ ========== */
    // HEAD
    public static int head(int[] arr) { return arr[0]; }
    public static float head(float[] arr) { return arr[0]; }
    public static String head(String[] arr) { return arr[0]; }

    // LAST
    public static int last(int[] arr) { return arr[arr.length - 1]; }
    public static float last(float[] arr) { return arr[arr.length - 1]; }
    public static String last(String[] arr) { return arr[arr.length - 1]; }

    // TAIL (возвращает копию без первого элемента)
    public static int[] tail(int[] arr) {
        if (arr.length == 0) return arr;
        return Arrays.copyOfRange(arr, 1, arr.length);
    }
    public static float[] tail(float[] arr) {
        if (arr.length == 0) return arr;
        return Arrays.copyOfRange(arr, 1, arr.length);
    }
    public static String[] tail(String[] arr) {
        if (arr.length == 0) return arr;
        return Arrays.copyOfRange(arr, 1, arr.length);
    }

    // CONS (:) (добавляет значение в массив)
    public static int[] cons(int head, int[] tail) {
        int[] result = new int[tail.length + 1];
        result[0] = head;
        System.arraycopy(tail, 0, result, 1, tail.length);
        return result;
    }

    public static float[] cons(float head, float[] tail) {
        float[] result = new float[tail.length + 1];
        result[0] = head;
        System.arraycopy(tail, 0, result, 1, tail.length);
        return result;
    }

    public static String[] cons(String head, String[] tail) {
        String[] result = new String[tail.length + 1];
        result[0] = head;
        System.arraycopy(tail, 0, result, 1, tail.length);
        return result;
    }

    // Проверка на пустой список
    public static boolean isNull(int[] arr) { return arr == null || arr.length == 0; }
    public static boolean isNull(int[][] arr) { return arr == null || arr.length == 0; }
    public static boolean isNull(float[] arr) { return arr == null || arr.length == 0; }
    public static boolean isNull(String[] arr) { return arr == null || arr.length == 0; }
    public static boolean isNull(Object[] arr) { return arr == null || arr.length == 0; }



    /* ========== УНИВЕРСАЛЬНЫЕ МЕТОДЫ ДЛЯ МНОГОМЕРНЫХ МАССИВОВ ========== */
    
    // HEAD
    public static Object head(Object[] arr) {
        return arr[0];
    }

    // LAST
    public static Object last(Object[] arr) {
        return arr[arr.length - 1];
    }

    // TAIL
    public static Object[] tail(Object[] arr) {
        if (arr.length == 0) return arr;
        return Arrays.copyOfRange(arr, 1, arr.length);
    }

    // CONS
    public static Object[] cons(Object head, Object[] tail) {
        // 1. Узнаем реальный тип элементов массива tail
        Class<?> componentType = tail.getClass().getComponentType();
        
        // 2. Создаем новый массив ТОГО ЖЕ ТИПА длиной N+1
        Object[] result = (Object[]) Array.newInstance(componentType, tail.length + 1);
        result[0] = head;
        System.arraycopy(tail, 0, result, 1, tail.length);
        
        return result;
    }
    
    // Взятие по индексу (!!)
    public static Object get(Object[] arr, int index) {
        return arr[index];
    }



    // === MAP ===
    // Сигнатура для байткода: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;)Ljava/lang/Object;
    public static Object map(String className, String methodName, Object list) throws Exception {
        if (list == null) return null; // Или пустой массив

        int length = Array.getLength(list);
        if (length == 0) return list;

        // 1. Находим метод
        // Мы предполагаем, что метод принимает 1 аргумент. 
        Method method = findMethod(className, methodName, 1);
        
        // 2. Узнаем тип возвращаемого значения метода, чтобы создать правильный массив результата
        Class<?> returnType = method.getReturnType();
        Object resultList = Array.newInstance(returnType, length);

        // 3. Пробегаем и вызываем
        for (int i = 0; i < length; i++) {
            Object item = Array.get(list, i);
            Object mapped = method.invoke(null, item);
            Array.set(resultList, i, mapped);
        }

        return resultList;
    }

    // === FOLD (foldl) ===
    // Сигнатура: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;
    public static Object fold(String className, String methodName, int acc, Object list) throws Exception {
        Object currentAcc = acc; // Боксинг произойдет здесь автоматически в Java
        int length = Array.getLength(list);
        Method method = findMethod(className, methodName, 2);

        for (int i = 0; i < length; i++) {
            currentAcc = method.invoke(null, currentAcc, Array.get(list, i));
        }
        return currentAcc;
    }

    public static Object fold(String className, String methodName, float acc, Object list) throws Exception {
        Object currentAcc = acc; 
        int length = java.lang.reflect.Array.getLength(list);
        java.lang.reflect.Method method = findMethod(className, methodName, 2);

        for (int i = 0; i < length; i++) {
            currentAcc = method.invoke(null, currentAcc, java.lang.reflect.Array.get(list, i));
        }
        return currentAcc;
    }

    // Хелпер для поиска метода (упрощенный, ищет первый публичный статик с таким именем)
    private static Method findMethod(String className, String methodName, int paramCount) throws Exception {
        Class<?> clazz = Class.forName(className);
        for (Method m : clazz.getMethods()) {
            if (m.getName().equals(methodName) && m.getParameterCount() == paramCount) {
                return m;
            }
        }
        throw new NoSuchMethodException(className + "." + methodName);
    }
}