class ExceptionBase {
	protected:
		char *_message;
	
	public:
		ExceptionBase();
		ExceptionBase(char *message);
		ExceptionBase(const ExceptionBase &original);
		virtual ~ExceptionBase();
		
		friend ostream & operator << (ostream &stream, const ExceptionBase &eb);
		void setMessage(char *message);
		char *getMessage();
}
