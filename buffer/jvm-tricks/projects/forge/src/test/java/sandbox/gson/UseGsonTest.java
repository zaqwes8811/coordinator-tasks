package sandbox.gson;

import com.google.common.base.Optional;
import com.google.gson.Gson;
import org.junit.Test;

import java.util.*;

public class UseGsonTest {

  @Test
	public void testMain() {
		// TODO Auto-generated method stub
		Gson gson = new Gson();
		Collection<Integer> ints = new ArrayList<Integer>();
		ints.add(9);
		ints.add(92);
		String json = gson.toJson(ints);
		//System.out.println(json);
	}
  
  private static class Val {
  	public Optional<Integer> oi = Optional.absent();
  	public Optional<Integer> oi_ref = Optional.of(8);  // не сереализуется
  };

  @Test
  public void testWithOptionalValue() {
  	Optional<Integer> oi = Optional.absent();
  	//System.out.println(new Gson().toJson(oi));
  	oi = Optional.of(8);
  	//System.out.println(new Gson().toJson(oi));
  	
  	Val v = new Val();
  	v.oi_ref = Optional.of(8);
  	//System.out.println(new Gson().toJson(v));
  }
}
