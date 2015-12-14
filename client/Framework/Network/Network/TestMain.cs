using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Network
{
    class TestMain
    {
        private static string c2s = @"
		.package {
			type 0 : integer
			session 1 : integer
		}

		handshake 1 {
			response {
				msg 0  : string
			}
		}

		get 2 {
			request {
				what 0 : string
			}
			response {
				result 0 : string
			}
		}

		set 3 {
			request {
				what 0 : string
				value 1 : string
			}
		}";

        private static string s2c = @"
        .package {
	        type 0 : integer
	        session 1 : integer
        }

        heartbeat 1 {}
        ";

        static Client mClient = new Client();

        static void Main()
        {
            mClient.Start("192.168.1.110", 8888, "package", s2c, c2s);

            SpObject spObject = new SpObject();
            spObject.Insert("what", "Manistein");

            mClient.SendRequest("get", spObject);
        }
    }
}
