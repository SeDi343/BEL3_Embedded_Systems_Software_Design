/* ! I/O Task XMC4500
 *
 * \description A simple I/O program for the XMC4500
 *
 * \author Markus
 *
 * \version Rev.: 01, 27.09.2017 - Created the c and h file for the project
 */

#include "001_task_io.h"

#define UART_TX P1_5
#define UART_RX P1_4

#define RX_FIFO_INITIAL_LIMIT 1
#define TX_FIFO_INITIAL_LIMIT 1

#define BUFFER_SIZE_PRINTF 255
#define BUFFER_SIZE_RX 64
#define STRING_LF 10
/******************************************************************** GLOBALS */
_Bool str_available = false;
char rx_buffer[BUFFER_SIZE_RX] = {0};
/******************************************************************** STRUCTURES */
XMC_GPIO_CONFIG_t uart_tx =
{
    .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
    .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
};

XMC_GPIO_CONFIG_t uart_rx =
{
    .mode = XMC_GPIO_MODE_INPUT_TRISTATE
};

XMC_UART_CH_CONFIG_t uart_config =
{
    .data_bits = 8U,
    .stop_bits = 1U,
    .baudrate = 9600U
};
/******************************************************************** FUNCTIONS */

/** \name _initUART0_CH0
 * \param none
 * \return none
 *
 * \brief This function initializes the UART0 interface channel 0 of the XMC4500
 * Furthermore it initializes the RX & TX FIFO of the UART interface, sets the correct
 * input function for the GPIO pins and enables the RX interrupt service routine for
 * the UART0 interface.
 **/
void _initUART0_CH0() {
    /* USIC channels initialization */
    XMC_UART_CH_Init(XMC_UART0_CH0, &uart_config);
    
    XMC_UART_CH_SetInputSource(XMC_UART0_CH0, XMC_UART_CH_INPUT_RXD, USIC0_C0_DX0_P1_4);
    
    /* FIFOs initialization for both channels:
     *  8 entries for TxFIFO from point 0, LIMIT=1
     *  8 entries for RxFIFO from point 8, LIMIT=7 (SRBI is set if all 8*data has been received)
     *  */
    XMC_USIC_CH_TXFIFO_Configure(XMC_UART0_CH0, 0, XMC_USIC_CH_FIFO_SIZE_8WORDS, TX_FIFO_INITIAL_LIMIT);
    XMC_USIC_CH_RXFIFO_Configure(XMC_UART0_CH0, 8, XMC_USIC_CH_FIFO_SIZE_8WORDS, RX_FIFO_INITIAL_LIMIT);
    
    /* Enabling events for TX FIFO and RX FIFO */
    XMC_USIC_CH_RXFIFO_EnableEvent(XMC_UART0_CH0,XMC_USIC_CH_RXFIFO_EVENT_CONF_STANDARD |
                                   XMC_USIC_CH_RXFIFO_EVENT_CONF_ALTERNATE);
    
    /* Connecting the previously enabled events to a Service Request line number */
    XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_UART0_CH0,XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_STANDARD,0);
    XMC_USIC_CH_RXFIFO_SetInterruptNodePointer(XMC_UART0_CH0,XMC_USIC_CH_RXFIFO_INTERRUPT_NODE_POINTER_ALTERNATE,0);
    
    /* Start USIC operation as UART */
    XMC_UART_CH_Start(XMC_UART0_CH0);
    
    /*Initialization of the necessary ports*/
    XMC_GPIO_Init(UART_TX,&uart_tx);
    XMC_GPIO_Init(UART_RX,&uart_rx);
    
    /*Configuring priority and enabling NVIC IRQ for the defined Service Request line number */
    NVIC_SetPriority(USIC0_0_IRQn,63U);
    NVIC_EnableIRQ(USIC0_0_IRQn);
    
    return;
}


/** \function USIC0_0_IRQHandler
 * \param none
 * \return none
 *
 * \brief This interrupt service routine is called, if the UART0 interface receives
 * a character. It stores the transmitted characters into an input buffer and
 * stops reading characters after sending '\n'. The maximum string length is
 * 64 characters.
 **/
void USIC0_0_IRQHandler(void) {
    static uint8_t rx_ctr = 0;
    uint16_t rx_tmp = 0;
    
    /* Read the RX FIFO till it is empty */
    while(!XMC_USIC_CH_RXFIFO_IsEmpty(XMC_UART0_CH0)) {
        rx_tmp = XMC_UART_CH_GetReceivedData(XMC_UART0_CH0);
        
        if((rx_tmp != STRING_LF) && (!str_available)) {
            rx_buffer[rx_ctr++] = rx_tmp;
        } else {
            rx_ctr = 0;
            str_available = true;
        }
    }
}

/** \function _uart_send_char
 * \param char c
 * \return always 0
 *
 * \brief This function sends a given character via the UART0 interface to the
 * workstation.
 **/
uint8_t _uart_send_char(char c) {
    while(XMC_USIC_CH_GetTransmitBufferStatus(XMC_UART0_CH0) == XMC_USIC_CH_TBUF_STATUS_BUSY);
    XMC_UART_CH_Transmit(XMC_UART0_CH0, c);
    
    return 0;
}

/** \function _uart_printf
 * \param char *fmt
 * \return 0 on success or ENOMEM on failure
 *
 * \brief This function prints a formatted string to the UART0 interface and transmits
 * it to the workstation.
 **/
uint8_t _uart_printf(char *fmt, ...){
    va_list arg_ptr;
    char buffer[BUFFER_SIZE_PRINTF];
    
    if(fmt == NULL)  {
        return ENOMEM;
    }
    
    va_start(arg_ptr, fmt);
    vsprintf(buffer, fmt, arg_ptr);
    va_end(arg_ptr);
    
    _uart_send_string(buffer);
    return 0;
}

/** \function _uart_send_string
 * \param char *str
 * \return 0 on success or ENOMEM on failure
 *
 * \brief This function takes a given input string and sends it via the
 *UART0 interface via channel 0 to the workstation.
 **/
uint8_t _uart_send_string(char *str) {
    if(str == NULL) {
        return ENOMEM;
    }
    
    for(int i=0;i<strlen(str);i++) {
        while(XMC_USIC_CH_GetTransmitBufferStatus(XMC_UART0_CH0) == XMC_USIC_CH_TBUF_STATUS_BUSY);
        XMC_UART_CH_Transmit(XMC_UART0_CH0, str[i]);
    }
    return 0;
}

/** \function _uart_get_string
 * \param char *str
 * \return 0 on success and ENOMEM on failure
 *
 * \brief This function waits for a transmitted string. !!! Attention !!! This
 * function blocks the program flow.
 **/
uint8_t _uart_get_string (char *str) {
    if(str == NULL) {
        return ENOMEM;
    }
    
    while(!str_available);
    memcpy(str, &rx_buffer, strlen(rx_buffer));
    memset(&rx_buffer, 0x00, BUFFER_SIZE_RX);
    
    str_available = false;
    return 0;
}

/******************************************************************** globals */
volatile int32_t ITM_RxBuffer = ITM_RXBUFFER_EMPTY;


FILE __stdout;
FILE __stdin;


/*!
 *  @brief Initialize the SWD/TPI/ITM debug modules to output debug messages
 *         via the Debug interface. This will enable the outputz of printf()
 *         and fputc() like messages.
 *         \par
 *         In order to enable the output add the following command at the GDB
 *         prompt (not needed when using the GNU ARM Plugin from within Eclipse)
 *
 *         (gdb) monitor SWO EnableTarget 16000000 0 1 0
 *
 *         Open up a separate terminal and connect to the TERMINAL OUTPUT
 *         CHANNEL of the Segger JLink.
 *
 *         $ telnet localhost 2333
 *
 *  @param  -
 *  @return -
 *  @note Checkout:
 *        (1) CMSIS Core Documentation found along the CMSIS sources:
 *            CMSIS-Core > Reference > Debug Access
 *        (2) ARM Cortex-M4 Processor Technical Reference Manual
 *        (3) ARMv7-M Architecture Reference Manual
 */
void initRetargetSwo(void)
{
    // P2.1: After a system reset, this pin selects HWO0 (DB.TDO/DB.TRACESWO)
    // PORT2->HWSEL ... clear bits [3:2]
    PORT2->HWSEL &= ~PORT2_HWSEL_HW1_Msk;
    // PORT2->HWSEL ... set bit [2] -> HWO0
    PORT2->HWSEL |= 0x01UL << PORT2_HWSEL_HW1_Pos;
    
    // Enable trace systems
    // Source: ARMv7-M Architecture Reference Manual
    // DEMCR = Debug Exception and Monitor Control Register
    //         TRCENA ... global enable for DWT and ITM blocks
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    // SWO baud rate prescaler value
    // ACPR = Asynchronous Clock Prescaler Register
    //        fSWO = fCPU / (SWOSCALAR + 1)
    //        fSWO = 1MHz (maximum speed JLINK Lite CortexM)
    //        fCPU = 120MHz
    TPI->ACPR = 119UL;
    // Asynchronous SWO, using NRZ encoding
    // Source: ARMv7-M Architecture Reference Manual
    // SPPR = Selected Pin Control Register
    //        0x0 ... Synchronous Trace Port Mode
    //        0x1 ... Asynchronous Serial Wire Output (Manchester)
    //        0x2 ... Asynchronous Serial Wire Output (NRZ)
    TPI->SPPR = 0x2UL;
    // Source: ARM Cortex-M4 Processor Technical Reference Manual
    // Source: infineonforums -> 358-ITM-printf-style-debugging-not-working
    // FFCR = Formatter and Flash Control Register
    //        0x0 ... continuous formatting disabled
    //        0x1 ... continuous formatting enabled
    TPI->FFCR &= ~TPI_FFCR_EnFCont_Msk;
    
    // Unlock ITM
    // Source: infineonforums -> 358-ITM-printf-style-debugging-not-working
    ITM->LAR = 0xC5ACCE55;
    // Source: ARMv7-M Architecture Reference Manual, Sec. C1-7
    // TCR = Trace Control Register
    //       ITMENA ... enable ITM
    ITM->TCR |= ITM_TCR_ITMENA_Msk;
    // Source: ARMv7-M Architecture Reference Manual, Sec. C1-7
    // TER = Trace Enable Register
    //       STIMENA ... Stimulus Port #N is enabled when bit STIMENA[N]
    ITM->TER = 0xFFFFFFFF;
}

/*!
 * @brief Retarget the printf() function to output via the ITM Debug module.
 *        This function relies on ITM_SendChar() defined in core_cm4.h of the
 *        CMSIS library.
 */
int _write(int file, char *buf, int nbytes)
{
    int i;
    
    for(i = 0; i < nbytes; i++) {
        ITM_SendChar((*buf++));
    }
    return nbytes;
}

/*!
 * @brief Output a single charcter via the ITM Debug module.
 */
int fputc(int ch, FILE *f) {
    return (ITM_SendChar(ch));
}

/*!
 * @brief Read a single character via the ITM module.
 *
 * @note  Doesn't seem to work on the XMC board so far!
 */
int fgetc(FILE *f) {
    while (ITM_CheckChar() != 1);
    return (ITM_ReceiveChar());
}


/*!
 * @brief Debug Error output via the ITM module.
 *
 * @note  Not implemented so far ...
 */
#if DEBUG
int ferror(FILE *f) {
    /* Your implementation of ferror */
    return EOF;
}
#endif

void wait(unsigned long delay) {
    while(delay--) {
        __NOP();
    }
}
