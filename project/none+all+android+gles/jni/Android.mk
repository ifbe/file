LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)




#libboot0
LOCAL_SRC_FILES += \
library/libboot0/app/ndk/main.c \
library/libboot0/app/ndk/serial.c \
library/libboot0/app/ndk/thread.c \
library/libboot0/app/memory.c \
library/libboot0/app/board.c \
library/libboot0/init/args.c \
library/libboot0/init/init.c \
library/libboot0/pwrclk.c

#libboot1
LOCAL_SRC_FILES += \
library/libboot1/prep/prep.term.c \
library/libboot1/prep/prep.default.c \
library/libboot1/prep/prep.myml.c \
library/libboot1/prep/prep.mython.c \
library/libboot1/loop/loop.exiter.c \
library/libboot1/loop/loop.poller.c \
library/libboot1/loop/loop.pulser.c \
library/libboot1/loop/loop.realer.c \
library/libboot1/loop/loop.waiter.c \
library/libboot1/std/stdin.c \
library/libboot1/std/stdout.c \
library/libboot1/std/stdev.c \
library/libboot1/std/stdrel.c \
library/libboot1/worker.c


#libhard0
ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
library/libhard0/bare/cpu/x86/x86.fp.c \
library/libhard0/bare/cpu/x86/x86.port.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
library/libhard0/bare/cpu/x64/x64.fp.c \
library/libhard0/bare/cpu/x64/x64.port.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
library/libhard0/bare/cpu/arm/arm.fp.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
library/libhard0/bare/cpu/arm64/arm64.fp.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
library/libhard0/bare/cpu/mips/mips.fp.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
library/libhard0/bare/cpu/mips64/mips64.fp.c
endif
LOCAL_SRC_FILES += \
library/libhard0/linux/linux.wire.bt.c \
library/libhard0/linux/linux.wire.i2c.c \
library/libhard0/linux/linux.wire.gpio.c \
library/libhard0/linux/linux.wire.pci.c \
library/libhard0/linux/linux.wire.spi.c \
library/libhard0/linux/linux.wire.usb.c \
library/libhard0/linux/linux.wire.wifi.c \
library/libhard0/device.c

#libhard1
LOCAL_SRC_FILES += \
library/libhard1/sensor/sensor.icm20789.c \
library/libhard1/sensor/sensor.l3gd20.c \
library/libhard1/sensor/sensor.lsm9ds1.c \
library/libhard1/sensor/sensor.mpu9250.c \
library/libhard1/sensor/sensor.ms5611.c \
library/libhard1/hwinfo/hwinfo.acpi.c \
library/libhard1/hwinfo/hwinfo.dts.c \
library/libhard1/driver.c

#libsoft0
LOCAL_SRC_FILES += \
library/libsoft0/linux/linux.flow.epoll.c \
library/libsoft0/linux/linux.flow.socket.c \
library/libsoft0/linux/linux.flow.ptmx.c \
library/libsoft0/linux/linux.flow.tty.c \
library/libsoft0/linux/linux.memory.file.c \
library/libsoft0/linux/linux.memory.folder.c \
library/libsoft0/linux/linux.system.process.c \
library/libsoft0/linux/linux.system.random.c \
library/libsoft0/linux/linux.system.signal.c \
library/libsoft0/linux/linux.system.time.c \
library/libsoft0/system.c

#libsoft1
LOCAL_SRC_FILES += \
library/libsoft1/libalgo/liblang/assembly/asm.x64.c \
library/libsoft1/libalgo/liblang/assembly/asm.arm.c \
library/libsoft1/libalgo/liblang/program/prog.purec.c \
library/libsoft1/libalgo/liblang/program/prog.cpp.c \
library/libsoft1/libalgo/liblang/lang.c \
library/libsoft1/libalgo/libstr/conv/conv.arg.c \
library/libsoft1/libalgo/libstr/conv/conv.ascii.c \
library/libsoft1/libalgo/libstr/conv/conv.bigint.c \
library/libsoft1/libalgo/libstr/conv/conv.file.c \
library/libsoft1/libalgo/libstr/conv/conv.num.c \
library/libsoft1/libalgo/libstr/conv/conv.url.c \
library/libsoft1/libalgo/libstr/conv/conv.utf8.c \
library/libsoft1/libalgo/libstr/count/count.item.c \
library/libsoft1/libalgo/libstr/count/count.line.c \
library/libsoft1/libalgo/libstr/diff/diff.cmp.c \
library/libsoft1/libalgo/libstr/diff/diff.wild.c \
library/libsoft1/libalgo/libstr/move/move.copy.c \
library/libsoft1/libalgo/libstr/move/move.swap.c \
library/libsoft1/libalgo/crypt/crypt.aes.c \
library/libsoft1/libalgo/crypt/crypt.des.c \
library/libsoft1/libalgo/crypt/crypt.rsa.c \
library/libsoft1/libalgo/encdec/encdec.base64.c \
library/libsoft1/libalgo/encdec/encdec.qrcode.c \
library/libsoft1/libalgo/graph/graph.forcedirected.c \
library/libsoft1/libalgo/graph/graph.kruskal.c \
library/libsoft1/libalgo/graph/graph.prim.c \
library/libsoft1/libalgo/hash/hash.crc8.c \
library/libsoft1/libalgo/hash/hash.crc16.c \
library/libsoft1/libalgo/hash/hash.crc32.c \
library/libsoft1/libalgo/hash/hash.md5.c \
library/libsoft1/libalgo/hash/hash.sha1.c \
library/libsoft1/libalgo/hash/hash.sha256.c \
library/libsoft1/libalgo/hash/hash.sha512.c \
library/libsoft1/libalgo/hash/hash.string.c \
library/libsoft1/libalgo/keyexch/keyexch.dh.c \
library/libsoft1/libalgo/keyexch/keyexch.ecdhe.c \
library/libsoft1/libalgo/linear/linear.disjoint.c \
library/libsoft1/libalgo/linear/linear.memory.c \
library/libsoft1/libalgo/linear/linear.queue.c \
library/libsoft1/libalgo/linear/linear.stack.c \
library/libsoft1/libalgo/matching/match.bm.c \
library/libsoft1/libalgo/matching/match.kmp.c \
library/libsoft1/libalgo/matching/match.rough.c \
library/libsoft1/libalgo/neural/neural.ann.c \
library/libsoft1/libalgo/neural/neural.cnn.c \
library/libsoft1/libalgo/neural/neural.rnn.c \
library/libsoft1/libalgo/notation/nota.calc.c \
library/libsoft1/libalgo/notation/nota.postfix.c \
library/libsoft1/libalgo/notation/nota.prefix.c \
library/libsoft1/libalgo/solve/solve.2048.c \
library/libsoft1/libalgo/solve/solve.chess.c \
library/libsoft1/libalgo/solve/solve.maze.c \
library/libsoft1/libalgo/solve/solve.pcbwire.c \
library/libsoft1/libalgo/solve/solve.rubik.c \
library/libsoft1/libalgo/solve/solve.snake.c \
library/libsoft1/libalgo/solve/solve.sudoku.c \
library/libsoft1/libalgo/solve/solve.tetris.c \
library/libsoft1/libalgo/solve/solve.xiangqi.c \
library/libsoft1/libalgo/sorting/sort.bubble.c \
library/libsoft1/libalgo/sorting/sort.gnome.c \
library/libsoft1/libalgo/sorting/sort.heap.c \
library/libsoft1/libalgo/sorting/sort.insert.c \
library/libsoft1/libalgo/sorting/sort.merge.c \
library/libsoft1/libalgo/sorting/sort.quick.c \
library/libsoft1/libalgo/sorting/sort.select.c \
library/libsoft1/libalgo/transform/trfm.cosine.c \
library/libsoft1/libalgo/transform/trfm.fourier.c \
library/libsoft1/libalgo/transform/trfm.laplace.c \
library/libsoft1/libalgo/tree/tree.binary.c \
library/libsoft1/libalgo/tree/tree.redblack.c \
library/libsoft1/libalgo/tree/tree.bplus.c \
library/libsoft1/libchem/chem.equation.c \
library/libsoft1/libfile/audio/audio.ape.c \
library/libsoft1/libfile/audio/audio.mp3.c \
library/libsoft1/libfile/audio/audio.ogg.c \
library/libsoft1/libfile/audio/audio.wav.c \
library/libsoft1/libfile/audio/tone.drum.c \
library/libsoft1/libfile/audio/tone.flute.c \
library/libsoft1/libfile/audio/tone.piano.c \
library/libsoft1/libfile/audio/tone.violin.c \
library/libsoft1/libfile/certificate/cert.pem.c \
library/libsoft1/libfile/certificate/cert.x509.c \
library/libsoft1/libfile/compress/comp.7z.c \
library/libsoft1/libfile/compress/comp.cpio.c \
library/libsoft1/libfile/compress/comp.gz.c \
library/libsoft1/libfile/compress/comp.tar.c \
library/libsoft1/libfile/compress/comp.zip.c \
library/libsoft1/libfile/document/doc.doc.c \
library/libsoft1/libfile/document/doc.docx.c \
library/libsoft1/libfile/document/doc.latex.c \
library/libsoft1/libfile/document/doc.pdf.c \
library/libsoft1/libfile/document/doc.ppt.c \
library/libsoft1/libfile/document/doc.pptx.c \
library/libsoft1/libfile/document/doc.xls.c \
library/libsoft1/libfile/document/doc.xlsx.c \
library/libsoft1/libfile/electronic/ee.brd.c \
library/libsoft1/libfile/electronic/ee.dsn.c \
library/libsoft1/libfile/electronic/ee.pcbdoc.c \
library/libsoft1/libfile/electronic/ee.schdoc.c \
library/libsoft1/libfile/executable/exec.elf.c \
library/libsoft1/libfile/executable/exec.macho.c \
library/libsoft1/libfile/executable/exec.pe.c \
library/libsoft1/libfile/filesystem/fs.ext.c \
library/libsoft1/libfile/filesystem/fs.fat.c \
library/libsoft1/libfile/filesystem/fs.hfs.c \
library/libsoft1/libfile/filesystem/fs.ntfs.c \
library/libsoft1/libfile/font/font.fon.c \
library/libsoft1/libfile/font/font.otf.c \
library/libsoft1/libfile/font/font.ttf.c \
library/libsoft1/libfile/model/model.3mf.c \
library/libsoft1/libfile/model/model.dwg.c \
library/libsoft1/libfile/model/model.stl.c \
library/libsoft1/libfile/parttable/pt.apple.c \
library/libsoft1/libfile/parttable/pt.gpt.c \
library/libsoft1/libfile/parttable/pt.mbr.c \
library/libsoft1/libfile/picture/pic.bmp.c \
library/libsoft1/libfile/picture/pic.flif.c \
library/libsoft1/libfile/picture/pic.heif.c \
library/libsoft1/libfile/picture/pic.jpg.c \
library/libsoft1/libfile/picture/pic.png.c \
library/libsoft1/libfile/picture/pic.webp.c \
library/libsoft1/libfile/vdisk/disk.raw.c \
library/libsoft1/libfile/vdisk/disk.vhd.c \
library/libsoft1/libfile/vdisk/disk.vmdk.c \
library/libsoft1/libfile/video/video.avi.c \
library/libsoft1/libfile/video/video.gif.c \
library/libsoft1/libfile/video/video.mkv.c \
library/libsoft1/libfile/video/video.mp4.c \
library/libsoft1/libfile/video/video.rmvb.c \
library/libsoft1/libfile/file.c \
library/libsoft1/libmath/libmath.bigint.c \
library/libsoft1/libmath/libmath.matrix.c \
library/libsoft1/libmath/libmath.notation.c \
library/libsoft1/libmath/libmath.prime.c \
library/libsoft1/libmath/libmath.quaternion.c \
library/libsoft1/libmath/libmath.vector.c \
library/libsoft1/libflow/0.phy/phy.bt.c \
library/libsoft1/libflow/0.phy/phy.eth.c \
library/libsoft1/libflow/0.phy/phy.raw.c \
library/libsoft1/libflow/0.phy/phy.wlan.c \
library/libsoft1/libflow/1.link/link.arp.c \
library/libsoft1/libflow/1.link/link.ipv4.c \
library/libsoft1/libflow/1.link/link.ipv6.c \
library/libsoft1/libflow/1.link/link.ipx.c \
library/libsoft1/libflow/1.link/link.pppoe.c \
library/libsoft1/libflow/1.link/link.slip.c \
library/libsoft1/libflow/2.ip/ip46.icmp.c \
library/libsoft1/libflow/2.ip/ip46.tcp.c \
library/libsoft1/libflow/2.ip/ip46.udp.c \
library/libsoft1/libflow/2.ip/ipx.spx.c \
library/libsoft1/libflow/3.udp/udp.dns.c \
library/libsoft1/libflow/3.udp/udp.hack.c \
library/libsoft1/libflow/3.udp/udp.quic.c \
library/libsoft1/libflow/3.udp/udp.stun.c \
library/libsoft1/libflow/3.udp/udp.tftp.c \
library/libsoft1/libflow/4.tcp/tcp.dl.c \
library/libsoft1/libflow/4.tcp/tcp.ftp.c \
library/libsoft1/libflow/4.tcp/tcp.fuckgfw.c \
library/libsoft1/libflow/4.tcp/tcp.http.c \
library/libsoft1/libflow/4.tcp/tcp.p2p.c \
library/libsoft1/libflow/4.tcp/tcp.party.c \
library/libsoft1/libflow/4.tcp/tcp.proxy.c \
library/libsoft1/libflow/4.tcp/tcp.rdp.c \
library/libsoft1/libflow/4.tcp/tcp.rtmp.c \
library/libsoft1/libflow/4.tcp/tcp.sql.c \
library/libsoft1/libflow/4.tcp/tcp.socks.c \
library/libsoft1/libflow/4.tcp/tcp.vnc.c \
library/libsoft1/libflow/4.tcp/tcp.ssh.c \
library/libsoft1/libflow/4.tcp/tcp.telnet.c \
library/libsoft1/libflow/4.tcp/tcp.tls1_2.c \
library/libsoft1/libflow/4.tcp/tcp.tls1_3.c \
library/libsoft1/libflow/4.tcp/tcp.ws.c \
library/libsoft1/libflow/ahrs/ahrs.ekf.c \
library/libsoft1/libflow/ahrs/ahrs.madgwick.c \
library/libsoft1/libflow/ahrs/ahrs.mahony.c \
library/libsoft1/libflow/ahrs/ahrs.only_g.c \
library/libsoft1/libflow/ahrs/ahrs.only_ga.c \
library/libsoft1/libflow/ahrs/ahrs.ukf.c \
library/libsoft1/libflow/stream/stream.help.control.c \
library/libsoft1/libflow/stream/stream.help.search.c \
library/libsoft1/libflow/stream/stream.help.vt100.c \
library/libsoft1/libflow/stream/stream.conv.str2fv.c \
library/libsoft1/libflow/stream/stream.conv.fv2str.c \
library/libsoft1/libflow/stream/stream.conv.qu2eu.c \
library/libsoft1/libflow/stream/stream.parse.gcode.c \
library/libsoft1/libflow/stream/stream.parse.mavlink.c \
library/libsoft1/libflow/stream/stream.parse.nema0183.c \
library/libsoft1/libflow/stream/stream.revise.recut.c \
library/libsoft1/libflow/stream/stream.revise.reline.c \
library/libsoft1/libflow/stream/stream.revise.reorder.c \
library/libsoft1/libflow/stream/stream.test.echo.c \
library/libsoft1/libflow/stream/stream.test.pump.c \
library/libsoft1/libflow/stream/stream.test.dbgf32.c \
library/libsoft1/libflow/stream/stream.test.dbghex.c \
library/libsoft1/libflow/stream/stream.test.goslow.c \
library/libsoft1/libflow/stream/stream.test.fftpcm.c \
library/libsoft1/libflow/stream/stream.test.fftrgb.c \
library/libsoft1/libflow/stream/stream.image.rotate.c \
library/libsoft1/libflow/stream/stream.image.scale.c \
library/libsoft1/artery.c

#libuser0
LOCAL_SRC_FILES += \
library/libuser0/r2v-sensor/sensor.ahrs.c \
library/libuser0/r2v-sensor/sensor.slam.c \
library/libuser0/r2v-sensor/sensor.lidar.c \
library/libuser0/r2v-sensor/sensor.radar.c \
library/libuser0/v2r-gadget/1602/gadget.1602.c \
library/libuser0/v2r-gadget/12864/gadget.12864.c \
library/libuser0/v2r-robot/bionic/robot-arm.c \
library/libuser0/v2r-robot/bionic/robot-dog.c \
library/libuser0/v2r-robot/bionic/robot-man.c \
library/libuser0/v2r-robot/bionic/robot-snake.c \
library/libuser0/v2r-robot/copter/copter-hexa.c \
library/libuser0/v2r-robot/copter/copter-octo.c \
library/libuser0/v2r-robot/copter/copter-quad.c \
library/libuser0/v2r-robot/factory/3d-printer.c \
library/libuser0/v2r-robot/factory/3d-router.c \
library/libuser0/v2r-robot/vehicle/car-bldc.c \
library/libuser0/v2r-robot/vehicle/car-step.c \
library/libuser0/v2r-robot/vehicle/car-toy.c \
library/libuser0/v2r-window/lib-gl/gldraw/gl.coop.c \
library/libuser0/v2r-window/lib-gl/gldraw/gl.easy.c \
library/libuser0/v2r-window/lib-gl/gldraw/gl.full.c \
library/libuser0/v2r-window/lib-gl/gldraw/gl.none.c \
library/libuser0/v2r-window/lib-gl/globj/globj.fbo.c \
library/libuser0/v2r-window/lib-gl/globj/globj.shader.c \
library/libuser0/v2r-window/lib-gl/globj/globj.texture.c \
library/libuser0/v2r-window/lib-gl/globj/globj.varray.c \
library/libuser0/v2r-window/lib-gl/globj/globj.vertex.c \
library/libuser0/v2r-window/lib-gl/gltar/gl41.target.color.c \
library/libuser0/v2r-window/lib-gl/gltar/gl41.target.depth.c \
library/libuser0/v2r-window/lib-gl/gltar/gl41.target.gbuffer.c \
library/libuser0/v2r-window/lib-gl/gltar/gl41.target.window.c \
library/libuser0/v2r-window/os-android/android.gles.c \
library/libuser0/v2r-speaker/os-android/android.speaker.c \
library/libuser0/r2v-camera/os-android/android.cam.c \
library/libuser0/r2v-micphone/os-android/android.mic.c \
library/libuser0/w2w-cmdline/android/android.stdio.c \
library/libuser0/w2w-gamepad/android/android.gamepad.c \
library/libuser0/w2w-systray/android/android.tray.c \
library/libuser0/supply.c

#libuser1
LOCAL_SRC_FILES += \
library/libuser1/forfile/html/html.data.c \
library/libuser1/forfile/html/html.help.c \
library/libuser1/forfile/json/json.c \
library/libuser1/forfile/xml/xml.c \
library/libuser1/forfile/zimu/zimu.c \
library/libuser1/forgpu/gl10/gl10.ctx.c \
library/libuser1/forgpu/gl10/gl10.draw.c \
library/libuser1/forgpu/gl10/gl10.fbo.c \
library/libuser1/forgpu/gl41/carve/carve3d.opaque.point.c \
library/libuser1/forgpu/gl41/carve/carve3d.opaque.line.c \
library/libuser1/forgpu/gl41/carve/carve3d.opaque.trigon.c \
library/libuser1/forgpu/gl41/carve/carve3d.solid.point.c \
library/libuser1/forgpu/gl41/carve/carve3d.solid.line.c \
library/libuser1/forgpu/gl41/carve/carve3d.solid.trigon.c \
library/libuser1/forgpu/gl41/carve/carve3d.texed.ascii.c \
library/libuser1/forgpu/gl41/carve/carve3d.texed.craft.c \
library/libuser1/forgpu/gl41/carve/carve3d.other.circuit.c \
library/libuser1/forgpu/gl41/carve/carve3d.other.group.c \
library/libuser1/forgpu/gl41/carve/carve3d.other.helper.c \
library/libuser1/forgpu/gl41/gl41.coop.c \
library/libuser1/forgpu/gl41/gl41.data.c \
library/libuser1/forcpu/rgba/draw/draw.line.c \
library/libuser1/forcpu/rgba/draw/draw.solid.c \
library/libuser1/forcpu/rgba/draw/draw.shape.c \
library/libuser1/forcpu/rgba/draw/draw.ascii.c \
library/libuser1/forcpu/rgba/draw/draw.utf8.c \
library/libuser1/forcpu/rgba/draw/draw.vt100.c \
library/libuser1/forcpu/rgba/rgba.c \
library/libuser1/forcpu/yuv/yuv.c \
library/libuser1/forcpu/tui/tui.c \
library/libuser1/forcpu/cli/cli.c \
library/libuser1/forspu/al10/al10.c \
library/libuser1/forspu/sl10/sl10.c \
library/libuser1/forspu/spu.pcm.c \
library/libuser1/therule/order/collide/collide.simple.c \
library/libuser1/therule/order/gravity/gravity.simple.c \
library/libuser1/therule/chaos/event/event.rts.c \
library/libuser1/therule/chaos/event/event.rpg.1rd.c \
library/libuser1/therule/chaos/event/event.rpg.3rd.c \
library/libuser1/therule/chaos/event/event.pick.ray.c \
library/libuser1/therule/chaos/event/event.pick.area.c \
library/libuser1/therule/chaos/event/event.cam.free.c \
library/libuser1/therule/chaos/event/event.cam.frus.c \
library/libuser1/themind/chip/pilot/pilot.car.c \
library/libuser1/themind/chip/pilot/pilot.drone.c \
library/libuser1/themind/chip/solve/solve.maze.c \
library/libuser1/themind/chip/solve/solve.rubik.c \
library/libuser1/themind/user/baby/baby.c \
library/libuser1/themind/user/gary/gary.c \
library/libuser1/themind/user/john/john.c \
library/libuser1/themind/user/kali/kali.c \
library/libuser1/themind/user/kirk/kirk.c \
library/libuser1/themind/user/lara/lara.c \
library/libuser1/themind/user/lily/lily.c \
library/libuser1/themind/user/musk/musk.c \
library/libuser1/themind/user/rose/rose.c \
library/libuser1/themind/user/yoda/yoda.c \
library/libuser1/thedata/1d/lib1d.c \
library/libuser1/thedata/1d/json/json.c \
library/libuser1/thedata/1d/latex/latex.c \
library/libuser1/thedata/1d/xml/xaml.c \
library/libuser1/thedata/1d/xml/xml.c \
library/libuser1/thedata/2d/lib2d.c \
library/libuser1/thedata/2d/photo/photo.blur.c \
library/libuser1/thedata/2d/photo/photo.channel.c \
library/libuser1/thedata/2d/photo/photo.convert.c \
library/libuser1/thedata/2d/photo/photo.gray.c \
library/libuser1/thedata/2d/photo/photo.mosaic.c \
library/libuser1/thedata/2d/photo/photo.rotate.c \
library/libuser1/thedata/2d/photo/photo.scale.c \
library/libuser1/thedata/3d/lib3d.c \
library/libuser1/thedata/3d/matrix/mat.local2world.c \
library/libuser1/thedata/3d/matrix/mat.world2camera.c \
library/libuser1/thedata/3d/matrix/mat.camera2ndc.c \
library/libuser1/thedata/3d/matrix/mat.aid.c \
library/libuser1/thedata/3d/intersect/intersect.obb.c \
library/libuser1/thedata/3d/intersect/intersect.ray.c \
library/libuser1/thedata/3d/intersect/intersect.rect.c \
library/libuser1/thedata/3d/intersect/intersect.sphere.c \
library/libuser1/thedata/4d/lib4d.c \
library/libuser1/thedata/4d/asset.circuit.c \
library/libuser1/thedata/4d/asset.function.c \
library/libuser1/thedata/4d/asset.html.c \
library/libuser1/thedata/4d/asset.image.c \
library/libuser1/thedata/4d/asset.model.c \
library/libuser1/thedata/4d/asset.sound.c \
library/libuser1/thedata/scene/biology/bio.evolution.c \
library/libuser1/thedata/scene/biology/bio.multiply.c \
library/libuser1/thedata/scene/biology/bio.plague.c \
library/libuser1/thedata/scene/chemic/chem.corrode.c \
library/libuser1/thedata/scene/electric/ee.analog.c \
library/libuser1/thedata/scene/electric/ee.digital.c \
library/libuser1/thedata/scene/factory/fac.pcb.c \
library/libuser1/thedata/scene/factory/fac.gcode.c \
library/libuser1/thedata/scene/factory/fac.gerber.c \
library/libuser1/thedata/scene/physic/phy.field.c \
library/libuser1/thedata/scene/physic/phy.fluid.c \
library/libuser1/thedata/scene/physic/phy.force.c \
library/libuser1/thedata/scene/neural/nn.cnn.c \
library/libuser1/thedata/scene/neural/nn.rnn.c \
library/libuser1/thedata/scene/uiux/uiux.border.c \
library/libuser1/thedata/scene/uiux/uiux.frame.c \
library/libuser1/thedata/scene/uiux/uiux.guide.c \
library/libuser1/thedata/scene/virt/virt.html.c \
library/libuser1/thedata/scene/virt/virt.xaml.c \
library/libuser1/thedata/scene/scene.c \
library/libuser1/thedata/world/real.c \
library/libuser1/thedata/world/virt.c \
library/libuser1/thedata/test/test.c \
library/libuser1/thedata/mine.c \
library/libuser1/theitem/baseunit/objalgo/algo.algo.c \
library/libuser1/theitem/baseunit/objalgo/algo.bintree.c \
library/libuser1/theitem/baseunit/objalgo/algo.bplus.c \
library/libuser1/theitem/baseunit/objalgo/algo.graph.c \
library/libuser1/theitem/baseunit/objalgo/algo.link.c \
library/libuser1/theitem/baseunit/objalgo/algo.sort.c \
library/libuser1/theitem/baseunit/objalgo/algo.mnist.c \
library/libuser1/theitem/baseunit/objbar/bar.health.c \
library/libuser1/theitem/baseunit/objbar/bar.list.c \
library/libuser1/theitem/baseunit/objbar/bar.nav.c \
library/libuser1/theitem/baseunit/objbar/bar.skill.c \
library/libuser1/theitem/baseunit/objbar/bar.tab.c \
library/libuser1/theitem/baseunit/objbar/bar.tag.c \
library/libuser1/theitem/baseunit/objchem/chem.ch4.c \
library/libuser1/theitem/baseunit/objchem/chem.h2o.c \
library/libuser1/theitem/baseunit/objchem/chem.diamond.c \
library/libuser1/theitem/baseunit/objchem/chem.graphene.c \
library/libuser1/theitem/baseunit/objchip/chip.common.battery.c \
library/libuser1/theitem/baseunit/objchip/chip.common.capacitor.c \
library/libuser1/theitem/baseunit/objchip/chip.common.inductor.c \
library/libuser1/theitem/baseunit/objchip/chip.common.resistor.c \
library/libuser1/theitem/baseunit/objchip/chip.connect.pin.c \
library/libuser1/theitem/baseunit/objchip/chip.connect.wire.c \
library/libuser1/theitem/baseunit/objchip/chip.diode.emitting.c \
library/libuser1/theitem/baseunit/objchip/chip.diode.schottky.c \
library/libuser1/theitem/baseunit/objchip/chip.diode.zener.c \
library/libuser1/theitem/baseunit/objchip/chip.source.current.c \
library/libuser1/theitem/baseunit/objchip/chip.source.voltage.c \
library/libuser1/theitem/baseunit/objchip/chip.transistor.bjt.c \
library/libuser1/theitem/baseunit/objchip/chip.transistor.finfet.c \
library/libuser1/theitem/baseunit/objchip/chip.transistor.mosfet.c \
library/libuser1/theitem/baseunit/objcode/code.call.c \
library/libuser1/theitem/baseunit/objcode/code.for.c \
library/libuser1/theitem/baseunit/objcode/code.if.c \
library/libuser1/theitem/baseunit/objcode/code.jump.c \
library/libuser1/theitem/baseunit/objcam/cam.ortho.c \
library/libuser1/theitem/baseunit/objcam/cam.free.c \
library/libuser1/theitem/baseunit/objcam/cam.vrglass.c \
library/libuser1/theitem/baseunit/objdxgl/fbo/fbo.glass.c \
library/libuser1/theitem/baseunit/objdxgl/fbo/fbo.mirror.c \
library/libuser1/theitem/baseunit/objdxgl/fbo/fbo.portal.c \
library/libuser1/theitem/baseunit/objdxgl/fbo/fbo.water.c \
library/libuser1/theitem/baseunit/objdxgl/light/light.direction.c \
library/libuser1/theitem/baseunit/objdxgl/light/light.point.c \
library/libuser1/theitem/baseunit/objdxgl/light/light.projector.c \
library/libuser1/theitem/baseunit/objdxgl/light/light.spot.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.curtain.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.field.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.ground.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.particle.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.skydome.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.terrain.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.texball.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.texbox.c \
library/libuser1/theitem/baseunit/objdxgl/dxgl.weather.c \
library/libuser1/theitem/baseunit/objgear/gear.bevel.c \
library/libuser1/theitem/baseunit/objgear/gear.helical.c \
library/libuser1/theitem/baseunit/objgear/gear.hypoid.c \
library/libuser1/theitem/baseunit/objgear/gear.spur.c \
library/libuser1/theitem/baseunit/objgear/gear.worm.c \
library/libuser1/theitem/baseunit/objhtml/html.a.c \
library/libuser1/theitem/baseunit/objhtml/html.audio.c \
library/libuser1/theitem/baseunit/objhtml/html.button.c \
library/libuser1/theitem/baseunit/objhtml/html.canvas.c \
library/libuser1/theitem/baseunit/objhtml/html.div.c \
library/libuser1/theitem/baseunit/objhtml/html.img.c \
library/libuser1/theitem/baseunit/objhtml/html.p.c \
library/libuser1/theitem/baseunit/objhtml/html.script.c \
library/libuser1/theitem/baseunit/objhtml/html.ul.c \
library/libuser1/theitem/baseunit/objhtml/html.video.c \
library/libuser1/theitem/baseunit/objmath/math.addsub.c \
library/libuser1/theitem/baseunit/objmath/math.muldiv.c \
library/libuser1/theitem/baseunit/objmath/math.exponent.c \
library/libuser1/theitem/baseunit/objmath/math.matrix.c \
library/libuser1/theitem/baseunit/objmath/math.differential.c \
library/libuser1/theitem/baseunit/objmath/math.integral.c \
library/libuser1/theitem/baseunit/objmath/math.sincos.c \
library/libuser1/theitem/baseunit/objmic/mic.mono.c \
library/libuser1/theitem/baseunit/objmic/mic.stereo.c \
library/libuser1/theitem/baseunit/objpack/pack.80211.c \
library/libuser1/theitem/baseunit/objpack/pack.eth.c \
library/libuser1/theitem/baseunit/objpack/pack.udp.c \
library/libuser1/theitem/baseunit/objpack/pack.tcp.c \
library/libuser1/theitem/baseunit/objtab/tab.karnaugh.c \
library/libuser1/theitem/baseunit/objtab/tab.truth.c \
library/libuser1/theitem/baseunit/objuiux/uiux.gbuffer.c \
library/libuser1/theitem/baseunit/objuiux/uiux.oscillo.c \
library/libuser1/theitem/baseunit/objuiux/uiux.picture.c \
library/libuser1/theitem/baseunit/objuiux/uiux.qrcode.c \
library/libuser1/theitem/baseunit/objuiux/uiux.spectrum.c \
library/libuser1/theitem/baseunit/objuiux/uiux.texmix.c \
library/libuser1/theitem/baseunit/objuiux/uiux.video.c \
library/libuser1/theitem/baseunit/objuiux/uiux.voxel.c \
library/libuser1/theitem/baseunit/objvkbd/vkbd.button.c \
library/libuser1/theitem/baseunit/objvkbd/vkbd.control.c \
library/libuser1/theitem/baseunit/objvkbd/vkbd.joystick.c \
library/libuser1/theitem/baseunit/objvkbd/vkbd.keyboard.c \
library/libuser1/theitem/baseunit/objvkbd/vkbd.slider.c \
library/libuser1/theitem/compound/thechip/chip.acdc.hbridge.c \
library/libuser1/theitem/compound/thechip/chip.acdc.rectifier.c \
library/libuser1/theitem/compound/thechip/chip.fet.pmos.c \
library/libuser1/theitem/compound/thechip/chip.fet.nmos.c \
library/libuser1/theitem/compound/thechip/chip.logic.not.c \
library/libuser1/theitem/compound/thechip/chip.logic.nand.c \
library/libuser1/theitem/compound/thechip/chip.logic.and.c \
library/libuser1/theitem/compound/thechip/chip.logic.nor.c \
library/libuser1/theitem/compound/thechip/chip.logic.or.c \
library/libuser1/theitem/compound/thechip/chip.logic.xor.c \
library/libuser1/theitem/compound/thechip/chip.logic.xnor.c \
library/libuser1/theitem/compound/thechip/chip.adder.half.c \
library/libuser1/theitem/compound/thechip/chip.adder.full.c \
library/libuser1/theitem/compound/thechip/chip.logic.autocmos.c \
library/libuser1/theitem/compound/thechip/chip.flipflop.dff.c \
library/libuser1/theitem/compound/thechip/chip.flipflop.jkff.c \
library/libuser1/theitem/compound/thechip/chip.flipflop.tff.c \
library/libuser1/theitem/compound/thechip/chip.ram.feedback.c \
library/libuser1/theitem/compound/thechip/chip.ram.dynamic.c \
library/libuser1/theitem/compound/thechip/chip.ram.static.c \
library/libuser1/theitem/compound/thechip/chip.tran.mux.c \
library/libuser1/theitem/compound/thechip/chip.tran.tg.c \
library/libuser1/theitem/compound/thechip/chip.tran.tristate.c \
library/libuser1/theitem/compound/thedxgl/cam.1rd.c \
library/libuser1/theitem/compound/thedxgl/cam.3rd.c \
library/libuser1/theitem/compound/thedxgl/cam.rts.c \
library/libuser1/theitem/compound/thefile/file.common.folder.c \
library/libuser1/theitem/compound/thefile/file.common.hexedit.c \
library/libuser1/theitem/compound/thefile/file.biology.dna.c \
library/libuser1/theitem/compound/thefile/file.biology.fasta.c \
library/libuser1/theitem/compound/thefile/file.model.geometry.c \
library/libuser1/theitem/compound/thefile/file.model.scad.c \
library/libuser1/theitem/compound/thefile/file.model.obj.c \
library/libuser1/theitem/compound/thefile/file.model.stl.c \
library/libuser1/theitem/compound/thefile/file.program.arm64.c \
library/libuser1/theitem/compound/thefile/file.program.mips64.c \
library/libuser1/theitem/compound/thefile/file.program.x86_64.c \
library/libuser1/theitem/compound/thefile/file.text.editor.c \
library/libuser1/theitem/compound/thefile/file.text.cpure.c \
library/libuser1/theitem/compound/thefile/file.text.cplus.c \
library/libuser1/theitem/compound/thefile/file.text.java.c \
library/libuser1/theitem/compound/thegame/chess/game.chess.c \
library/libuser1/theitem/compound/thegame/chess/game.gobang.c \
library/libuser1/theitem/compound/thegame/chess/game.kriegspiel.c \
library/libuser1/theitem/compound/thegame/chess/game.pegged.c \
library/libuser1/theitem/compound/thegame/chess/game.weiqi.c \
library/libuser1/theitem/compound/thegame/chess/game.xiangqi.c \
library/libuser1/theitem/compound/thegame/classic/game.brick.c \
library/libuser1/theitem/compound/thegame/classic/game.klotski.c \
library/libuser1/theitem/compound/thegame/classic/game.puzzle.c \
library/libuser1/theitem/compound/thegame/classic/game.snake.c \
library/libuser1/theitem/compound/thegame/classic/game.sokoban.c \
library/libuser1/theitem/compound/thegame/classic/game.tetris.c \
library/libuser1/theitem/compound/thegame/mahjong/mahjong.c \
library/libuser1/theitem/compound/thegame/poker/poker.c \
library/libuser1/theitem/compound/thegame/game.2048.c \
library/libuser1/theitem/compound/thegame/game.maze.c \
library/libuser1/theitem/compound/thegame/game.ooxx.c \
library/libuser1/theitem/compound/thegame/game.rubik.c \
library/libuser1/theitem/compound/thegame/game.sudoku.c \
library/libuser1/theitem/compound/theitem/item.clock.c \
library/libuser1/theitem/compound/theitem/item.house.c \
library/libuser1/theitem/compound/theitem/item.mario.c \
library/libuser1/theitem/compound/theitem/item.mobius.c \
library/libuser1/theitem/compound/theitem/item.motor.c \
library/libuser1/theitem/compound/theitem/item.piano.c \
library/libuser1/theitem/compound/theitem/item.planet.c \
library/libuser1/theitem/compound/theitem/item.satellite.c \
library/libuser1/theitem/compound/theitem/item.spring.c \
library/libuser1/theitem/compound/theitem/item.stick.c \
library/libuser1/theitem/compound/theitem/item.tardis.c \
library/libuser1/theitem/compound/theitem/item.tree.c \
library/libuser1/theitem/compound/themove/move.elevator.c \
library/libuser1/theitem/compound/themove/move.ladder.c \
library/libuser1/theitem/compound/themove/move.stair.c \
library/libuser1/theitem/compound/themove/move.telpher.c \
library/libuser1/theitem/compound/thereal/real.theear.c \
library/libuser1/theitem/compound/thereal/real.theeye.c \
library/libuser1/theitem/compound/thereal/real.screen.c \
library/libuser1/theitem/compound/thereal/real.speaker.c \
library/libuser1/theitem/compound/theredu/redu.cycloid.c \
library/libuser1/theitem/compound/theredu/redu.harmonic.c \
library/libuser1/theitem/compound/theredu/redu.planetary.c \
library/libuser1/theitem/compound/therobo/robo.balance.c \
library/libuser1/theitem/compound/therobo/robo.dancemat.c \
library/libuser1/theitem/compound/therobo/robo.dog.c \
library/libuser1/theitem/compound/therobo/robo.drone.c \
library/libuser1/theitem/compound/therobo/robo.human.c \
library/libuser1/theitem/compound/therobo/robo.otto.c \
library/libuser1/theitem/compound/therobo/robo.rccar.c \
library/libuser1/theitem/compound/thetest/test.calib3d.c \
library/libuser1/theitem/compound/thetest/test.codeimg.c \
library/libuser1/theitem/compound/thetest/test.doodle.c \
library/libuser1/theitem/compound/thetest/test.example.c \
library/libuser1/theitem/compound/thetest/test.fractal.c \
library/libuser1/theitem/compound/thetest/test.palette.c \
library/libuser1/theitem/compound/theuiux/hack.browser.c \
library/libuser1/theitem/compound/theuiux/hack.circuit.c \
library/libuser1/theitem/compound/theuiux/hack.editor.c \
library/libuser1/theitem/compound/theuiux/hack.filesystem.c \
library/libuser1/theitem/compound/theuiux/hack.monitor.c \
library/libuser1/theitem/compound/theuiux/hack.pwmtool.c \
library/libuser1/theitem/compound/theuiux/hack.sketchpad.c \
library/libuser1/theitem/compound/theuiux/hack.wireshark.c \
library/libuser1/theitem/compound/theuiux/tool.calculator.c \
library/libuser1/theitem/compound/theuiux/tool.font.c \
library/libuser1/theitem/compound/theuiux/tool.terminal.c \
library/libuser1/theitem/compound/theuiux/tool.terminal.helper.c \
library/libuser1/theitem/compound/theuiux/menu.overview.c \
library/libuser1/theitem/compound/theuiux/menu.detail.c \
library/libuser1/theitem/compound/theuiux/menu.login.c \
library/libuser1/theitem/compound/theuiux/uiux.pointer.c \
library/libuser1/theitem/compound/theuiux/uiux.corner.c \
library/libuser1/theitem/compound/theuiux/uiux.cameraer.c \
library/libuser1/theitem/compound/theuiux/uiux.layouter.c \
library/libuser1/entity.c




#2
LOCAL_C_INCLUDES := $(LOCAL_PATH)/library
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS := -lm -llog -ljnigraphics -landroid -lEGL -lGLESv3
LOCAL_MODULE := finalanswer
LOCAL_STATIC_LIBRARIES := android_native_app_glue

include $(BUILD_SHARED_LIBRARY)
$(call import-module,android/native_app_glue)
