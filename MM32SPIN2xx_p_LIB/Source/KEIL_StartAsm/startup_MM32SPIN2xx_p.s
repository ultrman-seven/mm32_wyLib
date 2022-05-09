;******************** (C) COPYRIGHT 2019 MindMotion ********************
;* file               : startup_MM32SPIN2xx_p.s
;* author             : AE Team
;* version            : V1.1.0
;* date               : 09/09/2019
;* Description        : Amount of memory (in bytes) allocated for Stack
;                       Tailor this value to your application needs
; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>


Stack_Size      EQU     0x00000400

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

;Heap_Size       EQU     0x00000200
Heap_Size       EQU     0x00000800

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset
                AREA    RESET, DATA, READONLY
                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp                   ; Top of Stack
                DCD     Reset_Handler                  ; Reset Handler
                DCD     NMI_Handler                    ; NMI Handler
                DCD     HardFault_Handler              ; Hard Fault Handler
                DCD     MemManage_Handler              ; MPU Fault Handler 
                DCD     BusFault_Handler               ; Bus Fault Handler
                DCD     UsageFault_Handler             ; Usage Fault Handler
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     0                              ; Reserved
                DCD     SVC_Handler                    ; SVCall Handler
                DCD     DebugMon_Handler               ; Debug Monitor Handler
                DCD     0                              ; Reserved
                DCD     PendSV_Handler                 ; PendSV Handler
                DCD     _Z15SysTick_Handlerv                ; SysTick Handler

; External Interrupts
                DCD     _Z15WWDG_IRQHandlerv                ; Window Watchdog
                DCD     _Z14PVD_IRQHandlerv                 ; PVD through EXTI Line detect
                DCD     _Z14PWM_IRQHandlerv                 ; Commutation and input error interrupt
                DCD     _Z16FLASH_IRQHandlerv               ; FLASH
                DCD     _Z14RCC_IRQHandlerv                 ; RCC 
                DCD     _Z18EXTI0_1_IRQHandlerv             ; EXTI Line 0 and 1
                DCD     _Z18EXTI2_3_IRQHandlerv             ; EXTI Line 2 and 3
                DCD     _Z19EXTI4_15_IRQHandlerv            ; EXTI Line 4 to 15
                DCD     _Z16HWDIV_IRQHandlerv               ; HWDIV
                DCD     _Z24DMA1_Channel1_IRQHandlerv       ; DMA1 Channel 1
                DCD     _Z26DMA1_Channel2_3_IRQHandlerv     ; DMA1 Channel 2 and Channel 3
                DCD     _Z26DMA1_Channel4_5_IRQHandlerv     ; DMA1 Channel 4 and Channel 5
                DCD     _Z15ADC1_IRQHandlerv                ; ADC1
                DCD     _Z30TIM1_BRK_UP_TRG_COM_IRQHandlerv ; TIM1 Break, Update, Trigger and Commutation
                DCD     _Z18TIM1_CC_IRQHandlerv             ; TIM1 Capture Compare
                DCD     _Z15TIM2_IRQHandlerv                ; TIM2
                DCD     _Z15TIM3_IRQHandlerv                ; TIM3
                DCD     _Z30TIM8_BRK_UP_TRG_COM_IRQHandlerv ; TIM8 Brake, Update, Trigger and Commutation
                DCD     _Z18TIM8_CC_IRQHandlerv             ; TIM8 Capture Compare
                DCD     _Z16TIM14_IRQHandlerv          ; TIM14
                DCD     _Z15ADC2_IRQHandlerv         ; ADC2
                DCD     _Z16TIM16_IRQHandlerv     ; TIM16
                DCD     _Z16TIM17_IRQHandlerv    ; TIM17
                DCD     _Z15I2C1_IRQHandlerv ; I2C1
                DCD     _Z24COMP1_2_3_4_5_IRQHandlerv   ; Comparators
                DCD     _Z15SPI1_IRQHandlerv            ; SPI1
                DCD     _Z15SPI2_IRQHandlerv            ; SPI2
                DCD     _Z16UART1_IRQHandlerv           ; UART1
                DCD     _Z16UART2_IRQHandlerv           ; UART2

                
__Vectors_End

__Vectors_Size  EQU  __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY

; Reset handler routine
Reset_Handler   PROC
                EXPORT  Reset_Handler                 [WEAK]
                IMPORT  __main
                IMPORT  SystemInit



                LDR     R0, =__initial_sp          ; set stack pointer 
                MSR     MSP, R0  

;;Check if boot space corresponds to test memory 

                LDR     R0,=0x00000004
                LDR     R1, [R0]
                LSRS    R1, R1, #24
                LDR     R2,=0x1F
                CMP     R1, R2
                        
                BNE     ApplicationStart  
     
ApplicationStart    
     
                LDR     R0,=0x4002101C 
                LDR     R1,=0x04000000
                STR     R1, [R0]
                        
                LDR     R0,=0x40006818 
                LDR     R1,=0x00000012
                STR     R1, [R0]
                        
                LDR     R0,=0x4000681C 
                LDR     R1,=0x00000012
                STR     R1, [R0]
                        
                LDR     R0,=0x4002101C 
                LDR     R1,=0x00000000
                STR     R1, [R0]
                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__main
                BX      R0
                ENDP

; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler                    [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler              [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler              [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler               [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler             [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler                    [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler               [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler                 [WEAK]
                B       .
                ENDP
_Z15SysTick_Handlerv PROC
                EXPORT  _Z15SysTick_Handlerv                [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  _Z15WWDG_IRQHandlerv                [WEAK]
                EXPORT  _Z14PVD_IRQHandlerv                 [WEAK]
                EXPORT  _Z14PWM_IRQHandlerv                 [WEAK]
                EXPORT  _Z16FLASH_IRQHandlerv               [WEAK]
                EXPORT  _Z14RCC_IRQHandlerv                 [WEAK]
                EXPORT  _Z18EXTI0_1_IRQHandlerv             [WEAK]
                EXPORT  _Z18EXTI2_3_IRQHandlerv             [WEAK]
                EXPORT  _Z19EXTI4_15_IRQHandlerv            [WEAK]
                EXPORT  _Z16HWDIV_IRQHandlerv               [WEAK]
                EXPORT  _Z24DMA1_Channel1_IRQHandlerv       [WEAK]
                EXPORT  _Z26DMA1_Channel2_3_IRQHandlerv     [WEAK]
                EXPORT  _Z26DMA1_Channel4_5_IRQHandlerv     [WEAK]
                EXPORT  _Z15ADC1_IRQHandlerv                [WEAK]
                EXPORT  _Z30TIM1_BRK_UP_TRG_COM_IRQHandlerv [WEAK]
                EXPORT  _Z18TIM1_CC_IRQHandlerv             [WEAK]
                EXPORT  _Z15TIM2_IRQHandlerv                [WEAK]
                EXPORT  _Z15TIM3_IRQHandlerv                [WEAK]
                EXPORT  _Z30TIM8_BRK_UP_TRG_COM_IRQHandlerv [WEAK]
                EXPORT  _Z18TIM8_CC_IRQHandlerv             [WEAK]
                EXPORT  _Z16TIM14_IRQHandlerv               [WEAK]
                EXPORT  _Z15ADC2_IRQHandlerv                [WEAK]
                EXPORT  _Z16TIM16_IRQHandlerv               [WEAK]
                EXPORT  _Z16TIM17_IRQHandlerv               [WEAK]
                EXPORT  _Z15I2C1_IRQHandlerv                [WEAK]
                EXPORT  _Z24COMP1_2_3_4_5_IRQHandlerv       [WEAK]
                EXPORT  _Z15SPI1_IRQHandlerv                [WEAK]
                EXPORT  _Z15SPI2_IRQHandlerv                [WEAK]
                EXPORT  _Z16UART1_IRQHandlerv               [WEAK]
                EXPORT  _Z16UART2_IRQHandlerv               [WEAK]



_Z15WWDG_IRQHandlerv
_Z14PVD_IRQHandlerv
_Z14PWM_IRQHandlerv
_Z16FLASH_IRQHandlerv
_Z14RCC_IRQHandlerv
_Z18EXTI0_1_IRQHandlerv
_Z18EXTI2_3_IRQHandlerv
_Z19EXTI4_15_IRQHandlerv
_Z16HWDIV_IRQHandlerv
_Z24DMA1_Channel1_IRQHandlerv
_Z26DMA1_Channel2_3_IRQHandlerv
_Z26DMA1_Channel4_5_IRQHandlerv
_Z15ADC1_IRQHandlerv
_Z30TIM1_BRK_UP_TRG_COM_IRQHandlerv
_Z18TIM1_CC_IRQHandlerv
_Z15TIM2_IRQHandlerv
_Z15TIM3_IRQHandlerv
_Z30TIM8_BRK_UP_TRG_COM_IRQHandlerv
_Z18TIM8_CC_IRQHandlerv
_Z16TIM14_IRQHandlerv
_Z15ADC2_IRQHandlerv
_Z16TIM16_IRQHandlerv
_Z16TIM17_IRQHandlerv
_Z15I2C1_IRQHandlerv
_Z24COMP1_2_3_4_5_IRQHandlerv
_Z15SPI1_IRQHandlerv
_Z15SPI2_IRQHandlerv
_Z16UART1_IRQHandlerv
_Z16UART2_IRQHandlerv


                B       .

                ENDP

                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
                IF      :DEF:__MICROLIB
                
                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit
                
                ELSE
                
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap
                 
__user_initial_stackheap

                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR

                ALIGN

                ENDIF

                END

;******************** (C) COPYRIGHT 2019 MindMotion ********************

