#ifndef _NAMING_H 
#define _NAMING_H
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//
#define PI 3.1415926535897932384626433832795028841971693993151
#define tau PI*2
//
#define _ent_ hex32('e','n','t',0)
#define _sup_ hex32('s','u','p',0)
#define _art_ hex32('a','r','t',0)
#define _sys_ hex32('s','y','s',0)
#define _dri_ hex32('d','r','i',0)
#define _dev_ hex32('d','e','v',0)
#define _wrk_ hex32('w','r','k',0)
#define _ori_ hex32('o','r','i',0)
//bare metal
#define _start_ hex64('s','t','a','r','t', 0, 0, 0)
#define _efimain_ hex64('e','f','i','m','a','i','n', 0)
//module
#define _lib42_ hex64('l','i','b','4','2', 0, 0, 0)
#define _kext_ hex32('k','e','x','t')
#define _ko_ hex32('k','o', 0, 0)
//application
#define _main_ hex32('m','a','i','n')
#define _ndkmain_ hex64('n','d','k','m','a','i','n', 0)
#define _win32_ hex64('w','i','n','3','2', 0, 0, 0)
#define _winmain_ hex64('w','i','n','m','a','i','n', 0)
//
#define _dbg_ hex32('d','b','g',0)
#define _i2c_ hex32('i','2','c',0)
#define _spi_ hex32('s','p','i',0)
#define _ptmx_ hex32('p','t','m','x')
#define _uart_ hex32('u','a','r','t')
#define _ahci_ hex32('a','h','c','i')
#define _xhci_ hex32('x','h','c','i')
#define _usb_ hex32('u','s','b',0)
//
#define _exiter_ hex64('e','x','i','t','e','r', 0, 0)
#define _poller_ hex64('p','o','l','l','e','r', 0, 0)
#define _pulser_ hex64('p','u','l','s','e','r', 0, 0)
#define _realer_ hex64('r','e','a','l','e','r', 0, 0)
#define _waiter_ hex64('w','a','i','t','e','r', 0, 0)
//
#define _src_ hex32('s','r','c',0)
#define _dst_ hex32('d','s','t',0)
#define _clk_ hex32('c','l','k',0)
//
#define _evby_ hex32('e','v','b','y')
#define _evto_ hex32('e','v','t','o')
#define _ioby_ hex32('i','o','b','y')
#define _ioto_ hex32('i','o','t','o')
//
#define _FILE_ hex32('F','I','L','E')
#define _file_ hex32('f','i','l','e')
#define _mbr_ hex32('m','b','r',0)
#define _gpt_ hex32('g','p','t',0)
//raw family
#define _RAW_ hex32('R','A','W',0)
#define _Raw_ hex32('R','a','w',0)
#define _raw_ hex32('r','a','w',0)
#define _ICMP_ hex32('I','C','M','P')
#define _icmp_ hex32('i','c','m','p')
#define _IGMP_ hex32('I','G','M','P')
#define _igmp_ hex32('i','g','m','p')
#define _ARP_ hex32('A','R','P',0)
#define _arp_ hex32('a','r','p',0)
#define _EAP_ hex32('E','A','P',0)
#define _eap_ hex32('e','a','p',0)
#define _IPX_ hex32('I','P','X',0)
#define _ipx_ hex32('i','p','x',0)
#define _PPPOE_ hex64('P','P','P','O','E',0,0,0)
#define _pppoe_ hex64('p','p','p','o','e',0,0,0)
#define _WOL_ hex32('W','O','L',0)
#define _wol_ hex32('w','o','l',0)
//
#define _UDP_ hex32('U','D','P',0)
#define _Udp_ hex32('U','d','p',0)
#define _udp_ hex32('u','d','p',0)
#define _TCP_ hex32('T','C','P',0)
#define _Tcp_ hex32('T','c','p',0)
#define _tcp_ hex32('t','c','p',0)
//
#define _dns_ hex32('d','n','s',0)
#define _Dns_ hex32('D','n','s',0)
#define _DNS_ hex32('D','N','S',0)
#define _ntp_ hex32('n','t','p',0)
#define _Ntp_ hex32('N','t','p',0)
#define _NTP_ hex32('N','T','P',0)
//
#define _bootp_ hex64('b','o','o','t','p',0,0,0)
#define _Bootp_ hex64('B','o','o','t','p',0,0,0)
#define _BOOTP_ hex64('B','O','O','T','P',0,0,0)
#define _dhcp_ hex32('d','h','c','p')
#define _Dhcp_ hex32('D','h','c','p')
#define _DHCP_ hex32('D','H','C','P')
#define _tftp_ hex32('t','f','t','p')
#define _Tftp_ hex32('T','f','t','p')
#define _TFTP_ hex32('T','F','T','P')
//
#define _webrtc_ hex64('w','e','b','r','t','c',0,0)
#define _Webrtc_ hex64('W','e','b','r','t','c',0,0)
#define _WEBRTC_ hex64('W','E','B','R','T','C',0,0)
#define _quic_ hex32('q','u','i','c')
#define _Quic_ hex32('Q','u','i','c')
#define _QUIC_ hex32('Q','U','I','C')
//
#define _HTTP_ hex32('H','T','T','P')
#define _Http_ hex32('H','t','t','p')
#define _http_ hex32('h','t','t','p')
#define _WS_   hex32('W','S',0,0)
#define _Ws_   hex32('W','s',0,0)
#define _ws_   hex32('w','s',0,0)
#define _TELNET_ hex64('T','E','L','N','E','T', 0, 0)
#define _Telnet_ hex64('T','e','l','n','e','t', 0, 0)
#define _telnet_ hex64('t','e','l','n','e','t', 0, 0)
#define _SSH_ hex32('S','S','H',0)
#define _Ssh_ hex32('S','s','h',0)
#define _ssh_ hex32('s','s','h',0)
#define _TLS1_2_  hex64('T','L','S','1','_','2', 0, 0)
#define _Tls1_2_  hex64('T','l','s','1','_','2', 0, 0)
#define _tls1_2_  hex64('t','l','s','1','_','2', 0, 0)
#define _TLS1_3_  hex64('T','L','S','1','_','3', 0, 0)
#define _Tls1_3_  hex64('T','l','s','1','_','3', 0, 0)
#define _tls1_3_  hex64('t','l','s','1','_','3', 0, 0)
//proxy
#define _PROXY_ hex64('P','R','O','X','Y', 0, 0, 0)
#define _Proxy_ hex64('P','r','o','x','y', 0, 0, 0)
#define _proxy_ hex64('p','r','o','x','y', 0, 0, 0)
#define _SOCKS_ hex64('S','O','C','K','S', 0, 0, 0)
#define _Socks_ hex64('S','o','c','k','s', 0, 0, 0)
#define _socks_ hex64('s','o','c','k','s', 0, 0, 0)
//
#define _PARTY_ hex64('P','A','R','T','Y', 0, 0, 0)
#define _Party_ hex64('P','a','r','t','y', 0, 0, 0)
#define _party_ hex64('p','a','r','t','y', 0, 0, 0)
#define _FUCKGFW_ hex64('F','U','C','K','G','F','W', 0)
#define _Fuckgfw_ hex64('F','u','c','k','g','f','w', 0)
#define _fuckgfw_ hex64('f','u','c','k','g','f','w', 0)
//trav
#define _UDPTRAV_ hex64('U','D','P','T','R','A','V', 0)
#define _Udptrav_ hex64('U','d','p','t','r','a','v', 0)
#define _udptrav_ hex64('u','d','p','t','r','a','v', 0)
#define _TCPTRAV_ hex64('T','C','P','T','R','A','V', 0)
#define _Tcptrav_ hex64('T','c','p','t','r','a','v', 0)
#define _tcptrav_ hex64('t','c','p','t','r','a','v', 0)
//http family
#define _HTTPS_ hex64('H','T','T','P','S', 0, 0, 0)
#define _Https_ hex64('H','t','t','p','s', 0, 0, 0)
#define _https_ hex64('h','t','t','p','s', 0, 0, 0)
#define _WSS_ hex32('W','S','S', 0)
#define _Wss_ hex32('W','s','s', 0)
#define _wss_ hex32('w','s','s', 0)
//live stream
#define _FLV_ hex32('F','L','V',0)
#define _Flv_ hex32('F','l','v',0)
#define _flv_ hex32('f','l','v',0)
#define _RTMP_ hex32('R','T','M','P')
#define _Rtmp_ hex32('R','t','m','p')
#define _rtmp_ hex32('r','t','m','p')
//remote desktop
#define _RDP_ hex32('R','D','P',0)
#define _Rdp_ hex32('R','d','p',0)
#define _rdp_ hex32('r','d','p',0)
#define _VNC_ hex32('V','N','C',0)
#define _Vnc_ hex32('V','n','c',0)
#define _vnc_ hex32('v','n','c',0)
#define _SPICE_ hex64('S','P','I','C','E', 0, 0, 0)
#define _Spice_ hex64('S','p','i','c','e', 0, 0, 0)
#define _spice_ hex64('s','p','i','c','e', 0, 0, 0)
//download
#define _ED2K_ hex32('E','D','2','K')
#define _Ed2k_ hex32('E','d','2','k')
#define _ed2k_ hex32('e','d','2','k')
#define _MAGNET_ hex64('M','A','G','N','E','T', 0, 0)
#define _Magnet_ hex64('M','a','g','n','e','t', 0, 0)
#define _magnet_ hex64('m','a','g','n','e','t', 0, 0)
#define _TORRENT_ hex64('T','O','R','R','E','N','T', 0)
#define _Torrent_ hex64('T','o','r','r','e','n','t', 0)
#define _torrent_ hex64('t','o','r','r','e','n','t', 0)
//
#define _SQL_ hex32('S','Q','L',0)
#define _Sql_ hex32('S','q','l',0)
#define _sql_ hex32('s','q','l',0)

//
#define _analog_ hex64('a','n','a','l','o','g', 0, 0)
#define _digital_ hex64('d','i','g','i','t','a','l', 0)
#define _eeworld_ hex64('e','e','w','o','r','l','d', 0)
//
#define _field_ hex64('f','i','e','l','d', 0, 0, 0)
#define _fluid_ hex64('f','l','u','i','d', 0, 0, 0)
#define _force_ hex64('f','o','r','c','e', 0, 0, 0)
//
#define _border2d_ hex64('b','o','r','d','e','r','2','d')
#define _frame3d_ hex64('f','r','a','m','e','3','d', 0)
#define _guide3d_ hex64('g','u','i','d','e','3','d', 0)
#define _scene3d_ hex64('s','c','e','n','e','3','d', 0)
//
#define _reality_ hex64('r','e','a','l','i','t','y', 0)
#define _virtual_ hex64('v','i','r','t','u','a','l', 0)
#define _mmio_ hex32('m','m','i','o')
#define _port_ hex32('p','p','r','t')
//
#define _htmlroot_ hex64('h','t','m','l','r','o','o','t')
#define _jsonroot_ hex64('j','s','o','n','r','o','o','t')
#define _xamlroot_ hex64('x','a','m','l','r','o','o','t')
#define _uiuxroot_ hex64('u','i','u','x','r','o','o','t')
//event
#define _follow_ hex64('f','o','l','l','o','w', 0, 0)
#define _lookat_ hex64('l','o','o','k','a','t', 0, 0)
#define _wander_ hex64('w','a','n','d','e','r', 0, 0)
#define _clickray_ hex64('c','l','i','c','k','r','a','y')
#define _event3rd_ hex64('e','v','e','n','t','3','r','d')
#define _eventrts_ hex64('e','v','e','n','t','r','t','s')
#define _touchcam_ hex64('t','o','u','c','h','c','a','m')
#define _touchobj_ hex64('t','o','u','c','h','o','b','j')
//dx
#define _dx11data_ hex64('d','x','1','1','d','a','t','a')
#define _dx11coop_ hex64('d','x','1','1','c','o','o','p')

#define _dx11wnd0_ hex64('d','x','1','1','w','n','d','0')
#define _dx11fbod_ hex64('d','x','1','1','f','b','o','d')
#define _dx11fboc_ hex64('d','x','1','1','f','b','o','c')
#define _dx11fbog_ hex64('d','x','1','1','f','b','o','g')
//gl
#define _gl41data_ hex64('g','l','4','1','d','a','t','a')
#define _gl41coop_ hex64('g','l','4','1','c','o','o','p')

#define _gl41fbo6_ hex64('g','l','4','1','f','b','o','6')
#define _gl41fboc_ hex64('g','l','4','1','f','b','o','c')
#define _gl41fbod_ hex64('g','l','4','1','f','b','o','d')
#define _gl41fbog_ hex64('g','l','4','1','f','b','o','g')
#define _gl41wnd0_ hex64('g','l','4','1','w','n','d','0')
//physic
#define _phys_ hex32('p','h','y','s')
#define _gravity_ hex64('g','r','a','v','i','t','y',0)
#define _collide_ hex64('c','o','l','l','i','d','e',0)
//sensor
#define _sensor_ hex64('s','e','n','s','o','r',0,0)
#define _lidar_ hex64('l','i','d','a','r',0,0,0)
#define _radar_ hex64('r','a','d','a','r',0,0,0)
#define _ahrs_ hex32('a','h','r','s')
#define _slam_ hex32('s','l','a','m')
//motor
#define _car_ hex32('c','a','r',0)
#define _bdc_ hex32('b','d','c',0)
#define _step_ hex32('s','t','e','p')
//sound
#define _mic_ hex32('m','i','c',0)
#define _spk_ hex32('s','p','k',0)
//light
#define _cam_ hex32('c','a','m',0)
#define _wnd_ hex32('w','n','d',0)
//
#define _cap_ hex32('c','a','p',0)
#define _holo_ hex32('h','o','l','o')
//
#define _tray_ hex32('t','r','a','y')	//tray
#define _std_ hex32('s','t','d', 0)		//cmdline
//
#define _w2w_ hex32('w','2','w', 0)
#define _khala_ hex64('k','h','a','l','a', 0, 0, 0)
#define _loopback_ hex64('l','o','o','p','b','a','c','k')
//tier=_sup_, type=_sup_, rfmt:
#define _none_ hex32('n','o','n','e')
#define _easy_ hex32('e','a','s','y')
#define _full_ hex32('f','u','l','l')
#define _coop_ hex32('c','o','o','p')
//tier=_sup_, type=_ctx_, rfmt:
#define _vbo_ hex32('v','b','o',0)
#define _rgba_ hex32('r','g','b','a')
#define _cli_ hex32('c','l','i',0)
#define _tui_ hex32('t','u','i',0)
#define _func_ hex32('f','u','n','c')
#define _sch_  hex32('s','c','h',0)
#define _pcb_  hex32('p','c','b',0)
#define _xml_  hex32('x','m','l',0)
//tier=_ent_, type:
#define _baby_ hex32('b','a','b','y')
#define _test_ hex32('t','e','s','t')
#define _pcm_ hex32('p','c','m',0)
#define _ctx_ hex32('c','t','x',0)
#define _8bit_ hex32('8','b','i','t')
//
#define _orig_ hex32('o','r','i','g')
#define _ORIG_ hex32('O','R','I','G')
#define _copy_ hex32('c','o','p','y')
#define _COPY_ hex32('C','O','P','Y')
//
#define _char_ hex32('c','h','a','r')
#define _drag_ hex32('d','r','a','g')
#define _joy_ hex32('j','o','y',0)
#define _kbd_ hex32('k','b','d',0)
#define _myml_ hex32('m','y','m','l')
#endif
