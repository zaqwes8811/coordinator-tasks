;/**
;  HOT- ����
;
;  ������ ����� Usart ������ ���
;*/
;NOTE:	����������� ���� ���������� ��������� T=85
#include <../headers/hot.inc>
mObject_var	_v2_HOT

home_data	res	0

;// ��������� ��� � KPUP �� ��� ��� �� ������������
DSFLAG		res		1
			;		DSFLAG,0 - H-Init is successful
			;		DSFLAG,1 - H-Last cicle is start( �� ���� �������� ������ ����������)
			;		DSFLAG,2 - H-Normal Work,L-first mal
			;		DSFLAG,3 - ����� �������
			;		DSFLAG,4 - H- DS- ������� (�������������) ����� DS (������� T=85, CRC=CRC_DS)
			;		DSFLAG,5 - H- DS- ����� ��� ������� ����� �������������� ��������������(3-� ������ �\���������)
			;		DSFLAG,6 - H- ������ ����� ������ �� DS
			;+060910 DSFLAG,7 - ������ ������� �������
DSTEMP		res		1	; ���� ����������� ->L avaData(_HOT)->M assiveUp+20(_BUM)->Resp_UM1+20(_INFO)->Mass2(_INFO)-> to PC
;/// 

DSLoop		res		1
CTDS		res 	1

;// ������ ��� ��� ���������� ������ �������� �����������
;//   � �� ��� ������ ��� �������� ������ �� ������� ������
LavaData	res 	1  
  ;//  movff	DSTEMP,LavaData ;  + 12	;+310110
  ;_HOT_GET_DS1821:	mObject_sel	THIS	movf	LavaData,w

DSbyte		res 	9
ctERR_DS	res		1		; ������� ERROR DS - ��� ����� ��4 ����������� �������� ����� 4���
							; ��� �������� �������� 0 �������� ������
ByteCount   res     1       ; ������� ���� ������
BitCount    res     1       ; ������� ��� � �����
DataByte    res     1       ; ���� ������
CRC         res     1       ; ��������� �������� CRC

;
xxxx idata
	waveFormTemper	res	1	; 0000_00ss
	hot_readOnly_zero	res	1; ����� ����� ��� ������ ��� ������

;/// /// ///
;///
;/// ������ ����
Obj_HOT	code

;// �������������
_HOT_SET_Ini:
		mObject_sel	THIS
		movlw	b'10001110'		;��������� ���� �����: ������� �������(,bit<0>) � �������� �������� ���� (bit<1>)
		movwf	DSFLAG			;DS1820PAR
		clrf	ctERR_DS		; +040310 ����� �������� ERROR DS
		
		; !!���� ��� ���� ���������� �������������� ���������
		banksel	waveFormTemper
		clrf	hot_readOnly_zero
		
		mObject_sel	THIS
	return

;/// /// ///
;///
;/// �/� ���������� � ������ ������������� ������ � DS1820PAR
;// ������ ����������� � �������, ����� ���������� �������
;_B UM_SETdw_Temper
;	_v#v(HERE)_T ASK_HLTuw_DataRefreshWait;(void/void)<<<DataRefreshWait
;		_C LK_HLTuw_Tick; ���������� ���������� �������
_HOT_SET_DS1821:
_DSread
_DSini_status

		mObject_sel	THIS
		btfsc	DSFLAG,0	;INI flag resetting		
			;�������� ��������� ������������ DS (�� 2-10�� - �� 5���)
		bra		out_DSini_status 
		clrf	DSTEMP		;��������� T=0 C

		incf	ctERR_DS,f	;+040310	��������� �������� ������ 
		movlw	4			;+040310
		cpfslt	ctERR_DS	;+040310  	���� 4 ���� ������?

		movff	DSTEMP,	LavaData ;  + 12	;+310110

out_DSini_status:

		btfsc	DSFLAG,3	; ++060910
		bra		_Testo
		btfsc	DSFLAG,1	;0- ���� �������� ������������� ��������������
		bra		_Tstart
		return
_Testo
		btfsc	DSFLAG,1	;0- ���� �������� ������������� ��������������
		bra		_Tstart
		btfsc	DSFLAG,5
		bra		_DSyes		;DS- ����� ��� ������� ����� �������������� ��������������(3-� ������ �\���������)
		call	_DSini
		btfss	DSFLAG,0
		return
		bsf		DSFLAG,5	;1-DS ����� ��� ������� ����� �������������� ��������������
		return

_DSyes 
		DelayUs	30;25
		movlw	0xCC		;command	��������� � DS1820
		call	_DSWbyte
		DelayUs	30;25
		movlw	0xBE		;command	������ t �� DS1820
		call	_DSWbyte
		DelayUs	13; 10		;~20mKs-���������?

		call	_DS_R9byte		;���� 9������ �� DS � ���������� ����� �����
		cpfseq	DSbyte+8		;�������� ����� �����
		bra		lbl_DSfalse_crc	;CRC NOT=DSbyte+8
		movff	DSbyte,DSTEMP

		nop						;����� ��� 1��� ��� ������� t, ��� �������� ������������ �����

		rrncf	DSTEMP		;����� �� 2 ������ ������� t=1 ������ �
		bcf		DSTEMP,7

lbl_85:						;��������� T=85, ��� ��������� ������ ������� DS:
		movlw	85			
		cpfseq	DSTEMP
		bra		lbl_85OUT
		bsf		DSFLAG,4		;H- ������� ����� DS
			incf	ctERR_DS	;+040310	��������� �������� ������ 
		clrf	DSTEMP			;T=0 C
			bra		lbl_out_DSyes2			;+040310
lbl_85OUT:	
		bcf	DSFLAG,4

		bra		lbl_out_DSyes

lbl_DSfalse_crc:
		incf	ctERR_DS		;+040310						
		clrf	DSTEMP			;T=0 C
		bsf		DSFLAG,6		;������ ����� �� DS
		bra		lbl_DS_in_Lava
lbl_out_DSyes:
		clrf	ctERR_DS		;+040310
lbl_out_DSyes2:
		bcf		DSFLAG,6		;not ������ ����� �� DS

lbl_DS_in_Lava:	;������ �������� �, ����������/������ �������� T=0			(�������� � ��������+040310)

		btfsc	DSFLAG,4		;H- ������� ����� DS ?
		bra		lbl_ctERR
		btfss	DSFLAG,6		;H-������ ����� �� DS?
		movff	DSTEMP,	LavaData ;+ 12 ;���������� �������� ����������� ������
lbl_ctERR:
		movlw		4			;+040310
		cpfslt		ctERR_DS	;+040310  	���� 4 ���� ������?
		movff	DSTEMP,LavaData ;+ 12			;+310110	 ��, ��������� �������� T=0 ����� 4-� ������ ������

		bsf		DSFLAG,1
		bcf	DSFLAG,5		;0-DS �� ����� ��� ������� ����� �������������� ��������������(���_DSini)
		return

_Tstart						;����� �������������� t � DS1820par
		mObject_sel	THIS	
		call	_DSini
		btfss	DSFLAG,0
		return
		DelayUs	30;25
		movlw	0xCC		;command
		call	_DSWbyte
		DelayUs	30;25		;� ���������� ����� ��������� ���
		movlw	0x44		;command �������������� �������������� � DS1820
		call	_DSWbyte
		bsf		TTX_port

		bcf		TTX_tris

		bcf		DSFLAG,1	;��������� ���� �������� - ��������

		bcf		DSFLAG,7	;��������� ���� ������ �������
		bcf		DSFLAG,3	;++ 060910

		return

_DSRbyte					;DS1821 BYTE Read Sub-Programm
		clrf	DSTEMP
		movlw	0x08
		movwf	DSLoop
_DSLoopr
		bcf		TTX_tris
		bcf		TTX_port
		DelayUs	3;2			;Really x2

		bcf		INTCON,GIEH
		bsf		TTX_tris
		DelayUs	12; 8
		btfsc	TTX_port

		bsf		DSTEMP,7

		bsf		INTCON,GIEH

		dcfsnz	DSLoop,f
		return				;Byte Reading is finished
		nop
		nop
		DelayUs	25 ;20 ;10 ;20 �����������!
		rrncf	DSTEMP,f
		bra		_DSLoopr

_DSWbyte					;DS1821 BYTE Write Sub-Programm
		movwf	DSTEMP

		movlw	0x09	;��� � ���� 9

		movwf	DSLoop
_DSLoopw
		dcfsnz	DSLoop,f
		return

		bcf		INTCON,GIEH

		bcf		TTX_tris
		bcf		TTX_port
		DelayUs	3; 2
		btfsc	DSTEMP,0	;Sending H?
		bsf		TTX_tris	;Yes, H
		DelayUs	75 ;60		;Send Slot Time
		bsf		TTX_tris

		bsf		INTCON,GIEH

		DelayUs	3; 2		;Pause 
		rrncf	DSTEMP,f
		bra		_DSLoopw

_DSini						;DS1821 INI Sub-Programm
		bcf		DSFLAG,0	;INI flag resetting
		bcf		TTX_tris
		bcf		TTX_port
		DelayUs	250; 200	;Really ~500mkS
		DelayUs	250; 200
		bsf		TTX_tris
		DelayUs	21; 15
		movlw	50
		movwf	CTDS
_DSW
		DelayUs	2;1
		dcfsnz	CTDS,f
		return
		btfsc	TTX_port
		bra		_DSW

		movlw	200
		movwf	CTDS
_DSW2
		DelayUs	2;1
		dcfsnz	CTDS,f
		return
		btfss	TTX_port
		bra		_DSW2
		bsf		DSFLAG,0	;INI is successful
		return

_DS_R9byte		;���� 9 ������ �� DS, ���������� CRC
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte+1
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte+2
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte+3
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte+4
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte+5
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte+6
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,DSbyte+7
		DelayUs	25 ; 20
		call	_DSRbyte
		MOVFF	DSTEMP,	DSbyte+8

		lfsr	0, 	DSbyte ;0x50		;����� _DSRbyte
		movlw	0x08			;��� ������ ��� ���������� CRC
;		call	CalcCRC8		;���������� CRC, return CRC in W


;   *********************************************************************
;   *                                                                   *
;   *                   ���������: CalcCRC8                             *
;   *                                                                   *
;   *    ���������: PIC16.                                              *
;   *    ���������: W = ����� ���� ������.                              *
;   *               FSR = ����� ������ ������.                          *
;   *               IRB = ����� ������ ������.                          *
;   *      �������: W = CRC8.                                           *
;   *     ��������: ���������� CRC8 �������� �� ����������� ��������    *
;   *               ���� X8+X5+X4+X1 (��. �������� DA LLAS AN27). CRC8   *
;   *               ����������� ��� ���������� � W ����� ������ ������, *
;   *               ������������� � ������, ����������� � ������ ������ *
;   *               �� ������, ���������� � �������� FSR � �����,       *
;   *               ������������ ����� IRB. ��������� ������������ � W. *
;   *               ��������� �������� � ������������ ����� � �����     *
;   *               ���� ��������� � ����� �������� ������ ��������.    *
;   *               ��������� ���������� ����������� � ����������       *
;   *               ������ ��� ������ ����� ������ ������.              *
;   *      �������: 26.11.2006 - �������� ����� CRC ��������            *
;   *               ������������ "#d efine CRC_POLY 0x18".               *
;   *                                                                   *
;   *********************************************************************

CalcCRC8:
        movwf   ByteCount       ; ���������� ����� ���������
        movlw   0x00            ; ������������� CRC ���� FF !!! -��� ����� ������� ������������������ ������
        movwf   CRC
CalcNextByte                    ; ������� ���� �� ������ ������
        movlw   8               ; ������������� ����������� ����� ��� 8
        movwf   BitCount        ;   ��� � �������� ���
        movf   POSTINC0,w       ; SLAVA 051010 ������� ���������� ����� ������
        movwf   DataByte        ;   � ��� �����������
CalcNextBit                     ; ���������� ���� �� ����� �����
        movf   CRC,w            ; ��������� �������� ���� CRC
        andlw   0x01
        xorwf   DataByte, F     ; DataByte.0 = ����� CRC.0
        btfss   DataByte, 0     ; �������, ���� ���� ������������� ���� 4 � 5 � CRC
        goto    ShftCRC        ; �������, ���� �� ���� ������������� ���� 4 � 5 � CRC

        movlw   CRC_POLY        ; �������������� ����� 4 � 5 � CRC
        xorwf   CRC, F
ShftCRC
        rrcf     DataByte, F     ; ����� ���� CRC
        rrcf     CRC, F

        decfsz  BitCount, F     ; ��������� � �������� �������� ���
        goto    CalcNextBit     ; ����, ���� �� ��� ���� ����� ����������

        decfsz  ByteCount, F    ; ��������� � �������� �������� ����
        goto    CalcNextByte    ; ����, ���� �� ��� ����� ������ ����������

        movf   	CRC,w           ; ������� ���������� ����� W
        return

;/// /// ///
;///
;/// ��� ����� �����-�� �������
_HOT_GET_DS1821:
		mObject_sel	THIS
		movf	LavaData,w
	return

;
_HOT_ASK_DS1821NeedTime:
		mObject_sel	THIS
		btfss	DSFLAG,7
		retlw	TRUE
		retlw	FALSE
;
_HOT_CLR_DS1821NeedTime:
		mObject_sel	THIS
		bsf		DSFLAG,7
	return
;
_HOT_SET_DS1821TimeOver:
		mObject_sel	THIS
		bsf		DSFLAG,3
	return


; igor
; ��������� �� ����� �����������
_v0_hot_getTermAlrmMask;(void/w)

	; �������� ���������
	banksel	waveFormTemper
	rlncf	waveFormTemper, f	; �������� ����� � ����� ������� 
	movlw	0x03	; 0000_0011 - �������
	andwf	waveFormTemper, f	; ������ �����
		; � 0-���� ���������� ���� - ������ �� ����

	mObject_sel	THIS
	; �������� �� ����� �� �����������
	movlw	TERM_THRESHOLD
	cpfsgt	LavaData	; f > w
	bra		temperAlrmNoAdded	; ��� ������, ���� ���������, ����� ��� �����������

	; ����� ���
	; ��������� � ��������� � �����
	banksel	waveFormTemper
	bsf	waveFormTemper, 0	; 0000_00x1

	; ��������� ��� ������
	movlw	b'00010000'
	bra		temperAlrmWasAdded

temperAlrmNoAdded:
	movlw	0
temperAlrmWasAdded:
	mRETURN



; �������� ������
; waveFormTemper = 0,1,2,3
;mEXTENDS	_v2_HOT,	_v?,	hot_public_testTempFront;(void/w)
hot_public_testTempFront;(void/w)
	banksel	waveFormTemper

	; switch (waveFormTemper) {
	; ���������� ���������
	; ��� � �� ���� ������ 0000_0000
	movlw	0x00
	cpfseq	waveFormTemper
		TSTFSZ	hot_readOnly_zero	; ����� ��������! � �� ����������
			retlw NO_EVENT

	; ��� ����� � �� ���������� 0000_0011
	movlw	0x03
	cpfseq	waveFormTemper
		TSTFSZ	hot_readOnly_zero
			retlw NO_EVENT

; Event occure
	; �������� �������������� ������ 0000_0001
	movlw	0x01
	cpfseq	waveFormTemper
		TSTFSZ	hot_readOnly_zero
			retlw EVENT_UP

	; �������� ����� 0000_0010
	movlw	0x02
	cpfseq	waveFormTemper
		TSTFSZ	hot_readOnly_zero
			mSlideTo	hot_prv_tAlrFalled;()
	;}
	retlw	NO_EVENT
; ����� ����
hot_prv_tAlrFalled;()
	;bcf 	INTCON,	GIE		;disable irq ��� ��� ���. ��������� ����� ��..
#ifdef Z_T_ALR_FALL
	;movlw	0x13
	;mCall setZ1
#endif
	mCall	onDisappearTemperAlrm;(void/void)
	retlw EVENT_DOWN
end