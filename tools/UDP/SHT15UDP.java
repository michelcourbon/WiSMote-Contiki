/*
 * Copyright (c) 2011, Plateforme Technologique de Valence.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
 * Simple class listening on 192.168.1.1:1234 (UDP) for TMP/HR data from a SHT1x
 * sensor.<br/>
 * <b>Format of the data:</b> tttt-hhhh (where t is for temperature and h for relative
 * humidity). We used the default precision (14 bits/12 bits) and a power supply of 3.5V.
 *
 * @author Plateforme Technologique de Valence - Anthony Gelibert
 * @version March 16, 2011
 */
public class SHT15UDP
{
    /* UDP buffers */
    /** UDP Buffer size. */
    private static final int BUFFER_SIZE = 10;
    /** UDP Buffer. */
    private static final byte[] BUFFER = new byte[BUFFER_SIZE];

    /* Temperature (14 bits / 3.5V) constants */
    private static final double D1 = -39.7;
    private static final double D2 = 0.01;

    /* Relative humidity (12 bits) constants */
    private static final double C1 = -2.0468;
    private static final double C2 = 0.0367;
    private static final double C3 = -0.0000015955;

    private SHT15UDP(){}

    public static void main(final String[] args) throws IOException
    {
        /* UDP socket. */
        final DatagramPacket data = new DatagramPacket(BUFFER, BUFFER_SIZE);
        final DatagramSocket socket = new DatagramSocket(1234, InetAddress.getByName("192.168.1.1"));
        System.out.println("I wait tmp/hr data on UDP port 1234\n");
        while (true)
        {
            socket.receive(data);
            final String[] values = new String(data.getData()).split("-", 2);
            try
            {
                final double tmp = D1 + D2 * Double.parseDouble(values[0]);
                System.out.println("TMP: " + round(tmp) + "°C");
            }
            catch (final NumberFormatException ex)
            {
                System.err.println("TMP: N/A");
            }
            try
            {
                final double rh = Double.parseDouble(values[1]);
                final double hr = C1 + C2 * rh + C3 * rh * rh;
                System.out.println("HR:  " + round(hr) + "%");
            }
            catch (final NumberFormatException ex)
            {
                System.err.println("HR:  N/A");
            }
            System.out.println("-------------");
        }
    }

    /**
     * Round a double to 2 decimal digits.
     *
     * @param d Original double.
     * @return Rounded version.
     */
    static double round(final double d)
    {
        return Math.rint(d * 100.0) / 100.0;
    }
}