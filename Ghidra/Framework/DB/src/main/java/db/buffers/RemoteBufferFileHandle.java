/* ###
 * IP: GHIDRA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package db.buffers;

import java.io.IOException;
import java.rmi.Remote;
import java.rmi.server.RemoteObjectInvocationHandler;
import java.util.NoSuchElementException;

/**
 * <code>RemoteBufferFileHandle</code> facilitates access to a remote BufferFile
 * via RMI.
 * <p>
 * Methods from {@link BufferFileHandle} <b>must</b> be re-declared here 
 * so they may be properly marshalled for remote invocation via RMI.  
 * This became neccessary with an OpenJDK 11.0.6 change made to 
 * {@link RemoteObjectInvocationHandler}.  
 */
public interface RemoteBufferFileHandle extends BufferFileHandle, Remote {
	@Override
	public boolean isReadOnly() throws IOException;

	@Override
	public boolean setReadOnly() throws IOException;

	@Override
	public int getParameter(String name) throws NoSuchElementException, IOException;

	@Override
	public void setParameter(String name, int value) throws IOException;

	@Override
	public void clearParameters() throws IOException;

	@Override
	public String[] getParameterNames() throws IOException;

	@Override
	public int getBufferSize() throws IOException;

	@Override
	public int getIndexCount() throws IOException;

	@Override
	public int[] getFreeIndexes() throws IOException;

	@Override
	public void setFreeIndexes(int[] indexes) throws IOException;

	@Override
	public void close() throws IOException;

	@Override
	public boolean delete() throws IOException;

	@Override
	public DataBuffer get(int index) throws IOException;

	@Override
	public void put(DataBuffer buf, int index) throws IOException;

	@Override
	public void dispose() throws IOException;

	@Override
	public InputBlockStream getInputBlockStream() throws IOException;

	@Override
	public OutputBlockStream getOutputBlockStream(int blockCount) throws IOException;

	@Override
	public BlockStreamHandle<InputBlockStream> getInputBlockStreamHandle() throws IOException;

	@Override
	public BlockStreamHandle<OutputBlockStream> getOutputBlockStreamHandle(int blockCount)
			throws IOException;

}
