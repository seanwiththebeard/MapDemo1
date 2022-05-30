.export _SIDINIT
.export _SIDFILE
.export _SIDPLAY
.export _SIDINIT
.export _STOPSID
_SIDFILE: .INCBIN "song.sid",$7e ;Pachelbel.sid, note not every SID will work, check after running sidreloc.exe to see where the load and play positions went

SIDLOADPOS = $AA00;
SIDPLAYPOS = $AA06;


_STOPSID:
	LDA #$00
	STA $D01A
        CLI
        RTS
SIDSTEP: 		;step SID
	jsr SIDPLAYPOS  ;jump to SIDPLAYPOS, return
	rts 

_SIDINIT:		;init SID 
	lda #0
	tax		;transfer accumilator to x register
	tay		;transfer accumilator to y register
	jsr SIDLOADPOS ;jump to SIDLOADPOS, return
	rts 

_SIDPLAY:
	LDX #<IRQ	;lo-byte
	LDY #>IRQ	;hi-byte
	STX $0314
	STY $0315

	LDA #$00	;raster pos
	STA $D012

	LDA #$7F	;cia enable interrupt
	STA $DC0D
	LDA #$1B	
	STA $D011	;vertical pos 
	LDA #$01
	STA $D01A	;raster input signals from vic (sync)
	LDA #$00
	CLI
	RTS

IRQ:
	INC $D019
	LDA #$00
	STA $D012	

	JSR SIDSTEP
        JMP $EA31