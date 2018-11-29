package us.pfrommer.insteon.msg;

import static org.junit.Assert.*;

import org.apache.commons.codec.binary.Hex;
import org.junit.Before;
import org.junit.Test;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class MsgReaderTest {

	private static final Logger logger = LoggerFactory.getLogger(MsgReaderTest.class);
	
	
	MsgReader reader;
	
	@Before
	public void beforeEachTest() {
		
		 reader = new MsgReader();
	}
	
	@Test
	public void testFPS() throws Exception {

	
		reader.addData(Hex.decodeHex("0262339DC3".toCharArray()), 0, 5 );
		reader.addData(Hex.decodeHex("1F2F0000000000000000000000000000D106".toCharArray()), 0, 18 );

		Msg actual = reader.processData();
		
		assertNotNull(actual);
	
		logger.debug("fps first message is {}", actual);
				
	}

}
