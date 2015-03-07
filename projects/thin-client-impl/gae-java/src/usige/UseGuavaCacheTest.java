package usige;


import com.google.common.cache.Cache;
import com.google.common.cache.CacheBuilder;
import com.google.common.cache.CacheLoader;
import com.google.common.cache.LoadingCache;
import junit.framework.TestCase;
import org.junit.Test;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutionException;

public class UseGuavaCacheTest extends TestCase {
  static class CacheHolder <K, V> {
    private final Cache<K, V> CACHE_;
    public CacheHolder(Cache<K, V> cache) {
      CACHE_ = cache;
    }

    public V get(K key) throws ExecutionException {
      return CACHE_.get(key, new Callable<V>() {
        @Override
        public V call() throws Exception {
          return hello();  //To change body of implemented methods use File | Settings | File Templates.
        }
      });
    }

    V hello() {
      return (V)"hello";
    }
  }

  @Test
  public void testInstall() throws ExecutionException {
    LoadingCache<String, String> graphs = CacheBuilder.newBuilder()
        .maximumSize(1000)
        .build(
            new CacheLoader<String, String>() {
              public String load(String key) throws Exception {
                return "hello";
              }
            });
    assertTrue(graphs.get("a").equals("hello"));
    assertTrue(graphs.get("b").equals("hello"));
  }
  @Test
  public void testInstallWithCallable() throws ExecutionException {
    Cache<String, String> cache = CacheBuilder.newBuilder()
      .maximumSize(1000)
      .build(); // look Ma, no CacheLoader

    // If the key wasn't in the "easy to compute" group, we need to
    // do things the hard way.
    CacheHolder<String, String> holder =
        new CacheHolder<String, String>(cache);
    assertTrue(holder.get("b").equals("hello"));
  }
}
